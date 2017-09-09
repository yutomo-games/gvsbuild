/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 2003 Harald Böhme
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  For more information, visit the MICO Home Page at
 *  http://www.mico.org/
 */

/* Modified by Cedric Gustin <cedric.gustin@gmail.com> on 2006/01/13 :
 * Redirect the output of dumpbin to dumpbin.out instead of reading the
 * output stream of popen, as it fails with Visual Studio 2005 in
 * pre-link build events.
 */

/* Modifief by Daniele Forghieri on 2017/09/09:
 *
 * Make a wrapper for the msvc link.exe command.
 *
 * This program scan the link command used, get the dll name, the def
 * file name and all the objects used and create the updated def file
 * using the utility dumpbin.
 *
 * After this the original link command (searched in the path) is called
 * changing the /_MK_DEF:file option to /DEF:file
 *
 * The net result is that all the definitions presents in the various
 * object file are exported and there is no need to declare them as
 * __declspec(dllexport), emulating gcc and the cmake's
 * WINDOWS_EXPORT_ALL_SYMBOLS
 */

// disable some CRT warnings
#define _CRT_SECURE_NO_WARNINGS

// include
#include "stdio.h"
//#include "stdlib.h"
//#include "string.h"
#include "process.h"
#include "io.h"
#include "sys\stat.h"

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

// old c '\0' terminated string
typedef char *  C_String;
// verbose level, 0 = off, max = 2
int verbose = 1;

// look for the original link.exe command on the path
string find_link(void)
{
    C_String      cpath;
    string      rt = "";

    cpath = getenv("PATH");
    while (*cpath != '\0') {
        C_String        p;
        struct stat     s_buffer;
        string          tested;
        string          full;

        p = strchr(cpath, ';');
        if (p != NULL) {
            int     l;

            l = p - cpath;
            tested.assign(cpath, l);
            cpath += l + 1;
        }
        else {
            tested.assign(cpath);
            cpath += strlen(cpath);
        }

        if (verbose > 1) {
            cout << "Checking " << tested << endl;
        }
        full = tested + "\\link.exe";
        if (stat(full.c_str(), &s_buffer) == 0) {
            // Esiste :)
            rt = full;
            if (verbose > 1) {
                cout << "Found!" << endl;
            }
            break;
        }

    }

    return(rt);

}

// look for the beginning of the string. If it's ok return the string after
// the beginning, generally a file name
string startsWith(C_String str, C_String part)
{
    unsigned int    l;

    l = strlen(part);
    if (strlen(str) < l) {
        return("");
    }

    if (_strnicmp(str, part, l) == 0) {
        return(str + l);
    }
    else {
        return("");
    }
}

// look for the end of the string, return true/false
bool endsWith(C_String str, C_String part)
{
    unsigned int    lp;
    unsigned int    ls;

    lp = strlen(part);
    ls = strlen(str);
    if (ls < lp) {
        return(false);
    }

    return (_stricmp(str + ls - lp, part) == 0 ? true : false);
}

// class to parse the link arguments
class parse_args {
    private:
        FILE        *m_fresp;
        C_String    m_resp_name;
        string      m_dllname;
        string      m_deffile;
        bool        m_mkdef;
        string      m_dumpbin;
        string      m_link_cmd;

        void close_resp(void);
        int make_def(void);
        int do_link(void);

    public:
        parse_args(string link_cmd);
        ~parse_args();

        void add_arg(C_String arg, bool quote = false);
        int execute(void);

};

parse_args::parse_args(string link_cmd)
{
    m_link_cmd = link_cmd;
    // init all
    m_resp_name = "xlink_cmd.rsp";
    m_fresp = fopen(m_resp_name, "wt");
    m_dllname = "dll_name";
    m_deffile = "_file.def";
    m_mkdef = false;
    m_dumpbin = "dumpbin /SYMBOLS /OUT:dumpbin.out";

}

parse_args::~parse_args()
{
    close_resp();
}

// close our response file
void parse_args::close_resp(void)
{
    if (m_fresp != nullptr) {
        fclose(m_fresp);
        m_fresp = nullptr;
    }
}

// add and check one single arg
void parse_args::add_arg(C_String ca, bool quote)
{

    string  part;
    string  add;

    add = ca;
    part = startsWith(ca, "/OUT:");
    if (part != "") {
        // dll/exe name
        size_t  cp;

        cp = part.rfind('\\');
        if (cp != string::npos) {
            part.assign(part, cp + 1);
        }
        cp = part.rfind('/');
        if (cp != string::npos) {
            part.assign(part, cp + 1);
        }
        // dll name
        m_dllname = part;

        if (verbose > 1) {
            cout << "dll name:" << m_dllname << endl;
        }
    }

    part = startsWith(ca, "/_MK_DEF:");
    if (part != "") {
        // our command to generate / use the .DEF file
        m_deffile = part;
        m_mkdef = true;
        add = "/DEF:" + part;
        quote = true;
        if (verbose > 1) {
            cout << "def file:" << m_deffile << endl;
        }
    }

    if (endsWith(ca, ".obj")) {
        // Obj for creating the .DEF file
        m_dumpbin += " ";
        m_dumpbin += ca;
        if (verbose > 1) {
            cout << "obj:" << add << endl;
        }
    }

    // build the original link response file
    if (quote) {
        fprintf(m_fresp, "\"%s\"\n", add.c_str());
    }
    else {
        fprintf(m_fresp, "%s\n", add.c_str());
    }
}

// create the .DEF file, if needed
int parse_args::make_def(void)
{
    if (!m_mkdef) {
        return(0);
    }

    FILE * dump;

    if ((dump = _popen(m_dumpbin.c_str(), "r")) == NULL) {
        cerr << "could not popen dumpbin" << endl;
        return 3;
    }

    // CG : Wait for the dumpbin process to finish and open dumpbin.out.
    _pclose(dump);
    dump = fopen("dumpbin.out", "r");

    ofstream def_file(m_deffile);

    def_file << "LIBRARY " << m_dllname << endl;
    def_file << "EXPORTS" << endl;

    while (!feof(dump)) {
        char buf[65000];

        if (fgets(buf, 64999, dump) != NULL) {
            if (!strstr(buf, " UNDEF ") && strstr(buf, " External ")) {
                char *s = strchr(buf, '|') + 1;
                while (*s == ' ' || *s == '\t') s++;
                char *e = s;
                while (*e != ' ' && *e != '\t' && *e != '\0' && *e != '\n') e++;
                *e = '\0';

                if (strchr(s, '?') == 0 && s[0] == '_' && strchr(s, '@') == 0)//this is a C export type: _fct -> fct
                    def_file << "    " << (s + 1) << endl;
                else
                    if (strchr(s, '?') != 0 && strncmp(s, "??_G", 4) != 0 && strncmp(s, "??_E", 4) != 0) {
                        def_file << "    " << s << endl;
                    }
            }
        }
    }

    // CG : Close dumpbin.out and delete it.
    fclose(dump);
    remove("dumpbin.out");

    if (verbose > 1) {
        cout << m_dumpbin.c_str() << endl;
    }
    else if (verbose > 0) {
        cout << "Definition file created: " << m_deffile << endl;
    }
    return(0);
}

// execute the original link command, eventually with the args changed
int parse_args::do_link()
{

    string  arg1;

    close_resp();
    arg1 = "@";
    arg1 += m_resp_name;
    return(_spawnl(_P_WAIT, m_link_cmd.c_str(), m_link_cmd.c_str(), arg1.c_str(), NULL));

}

// finalize the parsing, executing everything
int parse_args::execute(void)
{
    int     rt;

    rt = make_def();
    if (rt == 0) {
        rt = do_link();
    }
    return(rt);
}

int main(int argc, C_String argv[])
{
    string      link_cmd;
    int         i;

    // look for the linker
    link_cmd = find_link();
    if (link_cmd == "") {
        cerr << "Link command not found!" << endl;
        return(1);
    }

    if (verbose > 1) {
        cout << "Link found at " << link_cmd << endl;
    }

    // costruct the response file
    parse_args  pa(link_cmd);

    for(i = 1; i < argc; i++) {
        C_String  ca;

        ca = argv[i];
        if (*ca == '@') {
            // from file
            FILE    *fi;

            fi = fopen(ca + 1, "rt");
            if (fi != NULL) {
                int         max_size;
                C_String    line;

                max_size = _filelength(_fileno(fi));
                line = (C_String)calloc(1, max_size);
                while (fgets(line, max_size, fi) != NULL) {
                    C_String    p;
                    C_String    cl;

                    p = strchr(line, '\n');
                    if (p != NULL) {
                        *p = '\0';
                    }

                    cl = line;
                    while (true) {
                        // skip starting space(s)
                        while (*cl == ' ') {
                            ++cl;
                        }

                        if (*cl == '\0') {
                            break;
                        }

                        // quoting ?
                        if (*cl == '"') {
                            p = strchr(cl + 1, '"');
                            if (p != NULL) {
                                *p = '\0';
                                pa.add_arg(cl + 1);
                                cl = p + 1;
                            }
                            else {
                                // w.t.f. ?
                                pa.add_arg(cl);
                                break;
                            }
                        }
                        else {
                            p = strchr(cl, ' ');
                            if (p != NULL) {
                                *p = '\0';
                                pa.add_arg(cl);
                                cl = p + 1;
                            }
                            else {
                                pa.add_arg(cl);
                                break;
                            }
                        }

                    }
                }
                free(line);
                fclose(fi);
            }
        }
        else {
            // single argument
            pa.add_arg(ca);
        }

    }
    pa.execute();
    return 0;
}
