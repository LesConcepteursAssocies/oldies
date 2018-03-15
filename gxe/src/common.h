/*
** ----------------------------------------------------------------------------
** Gnu Xml Engine
** philippe Guillaumaud <pguillaumaud@april.org>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
**
** headers communs
*/
#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <syslog.h>
#include <dirent.h>

#include <errno.h>
#include <dlfcn.h>

#include <signal.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/param.h>
#include <arpa/inet.h>

#include <pthread.h>

/* XML */
#include <xmlmemory.h>
#include <parser.h>
#include <tree.h>
#include <parserInternals.h>

/* GLIB */
#include <glib.h>
#include <gmodule.h>

#endif
/*
** ----------------------------------------------------------------------------
*/
