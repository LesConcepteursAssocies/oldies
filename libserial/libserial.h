/*
** ----------------------------------------------------------------------------
** libserial
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
**
** Outils de manipulation des ports séries
*/
#ifndef LIBSERIAL_H
#define LIBSERIAL_H

#define  SL_LENBUFFER         1024

#define  SL_DFLTTIMEOUT       5

#define  SL_SUCCESSFUL        0
#define  SL_TIMEOUT           -1
#define  SL_QUEUEISEMPTY      -2
#define  SL_TERMINATORREACHED -3
#define  SL_ERROR             -99

/*
** Structure relative aux ports de communication
*/
struct _SLPParam
{
  int     speed;
  int     parity;
  int     bdata;
  int     bstop;
};

struct _SLPorts
{
  char              dev[PATH_MAX];           /* Nom du device attaché */
  int               fd;                      /* Descripteur du fichier */
  struct _SLPParam  param;                   /* Paramètres du port */
  struct termios    cfg_io;                  /* Config actuelle */
  struct termios    old_io;                  /* Config a l'ouverture */
  int               timeout;                 /* Timeout sur I/O */
  int               breceive;                /* 1 si on a reçu quelque chose sur ce port (utilisé par slWaitReceiving */
};

typedef struct _SLPorts *SLPortPtr;

/*
** Prototypes
*/
int   slOpenPort(SLPortPtr *,char *,int);
int   slClosePort(SLPortPtr *,int);
int   slSetPortAttr(SLPortPtr *,int,int,int,int);
int   slSetTimeout(SLPortPtr *,int);
char *slGetDeviceName(SLPortPtr *);
int   slGetFileDesc(SLPortPtr *);
int   slGetSpeed(SLPortPtr *);
int   slGetParity(SLPortPtr *);
int   slGetLenData(SLPortPtr *);
int   slGetBitStop(SLPortPtr *);
int   slGetTimeout(SLPortPtr *);
int   slGetbReceive(SLPortPtr *);
int   slFlushTransmitQueue(SLPortPtr *);
int   slFlushReceiveQueue(SLPortPtr *);
int   slWaitReceiving(SLPortPtr [],int,int);
int   slReceiveBuffer(SLPortPtr *,unsigned char *,int,unsigned char,int *);
int   slReceiveChar(SLPortPtr *);
int   slTransmitBuffer(SLPortPtr *,unsigned char *,int,int);

#endif
/*
** ----------------------------------------------------------------------------
*/
