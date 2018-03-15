// ีอออออออออออออัออออออออออออออออออตNEOSERVฦออออออออออออออออออออออออออออออออออธ
// ณSYMCCI.C     ณ                                                             ณ
// รฤฤฤฤฤฤฤฤฤฤฤฤฤู                                                             ณ
// ณAuteur                : Guillaumaud Philippe                               ณ
// ณ                        teknik@yaka2.com                                   ณ
// ณ                        http://www.yaka2.com/teknik                        ณ
// ณ                                                                           ณ
// ณVous tes autoris … diffuser et … utiliser ce logiciel sans restrictions. ณ
// ณ                                                                           ณ
// ณCe logiciel est fourni tel quel, sans aucune garantie.                     ณ
// ณL'auteur ne saurait voir sa responsabilit engage en cas de               ณ
// ณdommages de quelque nature que ce soit subis par l'utilisateur             ณ
// ณou des tiers et rsultant directement ou                                   ณ
// ณindirectement de son utilisation, notamment la perte de donnes,           ณ
// ณou toute perte financire rsultant de son                                 ณ
// ณutilisation ou de l'impossibilit de l'utiliser, et ceci mme si           ณ
// ณl'auteur a t prvenu de la possibilit de tels dommages.                 ณ
// ณ                                                                           ณ
// ณCommenc le           : 09/06/1993                                         ณ
// ณDernire modification : 31/07/1995                                         ณ
// ณ                                                                           ณ
// ณGestion du driver CX25 pour cartes MITEC.                                  ณ
// ิอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <dir.h>
#include <ctype.h>
#include <conio.h>
#include <int.h>
#include <io.h>
#include <share.h>
#include <fcntl.h>
#include <time.h>
#include <sys\stat.h>
#include "generic.h"
#include "symcci.h"

/*
** STRUCTURE DE GESTION D'UNE CARTE
*/
typedef struct
{
	int			p_flags;			/* Flags													*/
	unsigned long			p_segment;		/* Segment de base									*/
	unsigned		p_size;			/* Longueur de la mmoire							*/
   LPXPTR      p_xptr;        /* pointeur mode protg                     */
	mtPTRBYTE	p_base;			/* ^ base de la mmoire partage					*/
	mtPTRBYTE	p_zalt;			/* ^ table des alerte en MP						*/
	mtPTRBYTE   p_rcmd;			/* ^ registre de commande en MP					*/
	mtPTRBYTE	p_rsta;			/* ^ registre de status en MP						*/
	LPINFO		p_zinf;			/* ^ structure INF en MP							*/
	int			p_sdat;			/* Longueur utile										*/
	mtBYTE			p_iosta[256];	/* Copie du poids fort du status					*/
	mtBYTE			p_txsta[256];	/* Copie status d'emission							*/
	stat_t		p_stat;			/* Statistiques d'utilisation de la carte		*/
} pcb_t;

typedef  pcb_t          *LPCARTE;

/*
** STRUCTURE DE TAVAIL
*/
typedef struct
{
	int			z_dev;			/* Numero de device									*/
	int			z_noc;			/* Numero de carte									*/
	int			z_cnl;			/* Numero de canal									*/
	LPCARTE		z_pcb;			/* ^ sur l'entre de CCI_pcbtab					*/
	union
	{
		LPREQ	r;
		LPRIO	s;
	}				z_req;			/* ^ sur l'entete de la requete					*/
	mtPTRBYTE	z_dat;			/* ^ sur les donnes de la requte				*/
} zt_t;

typedef  zt_t           *LPZT;

/* Fonctions d'usage interne
*/
static int InitZt(LPZT);
static int SendReq(LPZT);

/*
** VARIABLES LOCALES
*/

int		CCI_InitError;				/*	=	0	initialisation correcte
											   !=	0	code d'erreur								*/

static int		CCI_maxpcb		=	4;
static int		CCI_maxdev		=	4 * 256;
static pcb_t	CCI_pcbtab[4];
static int		CCI_ret;

// Codes d'erreur d'initialisation.
#define  CCIER_NOERROR  0
#define  CCIER_OPENFILE 1
#define  CCIER_READFILE 2
#define  CCIER_MKPTR    3
#define  CCIER_ERRMEM   4
#define  CCIER_ERRDRV   5

//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialisation des cartes.
//
mtBOOL CCIInitCartes(void)
{
   int err,hConf,ncarte;
   long len;
   time_t tm;
   mtBYTE tmp;
   cfg_t cfg;
   char *pCfg;
   unsigned ladr;
   LPCARTE pcbp;

   CCI_ret = CCIER_NOERROR;
   pCfg    = searchpath("\\CX25\\X25CFG.SYS");
   if(pCfg)
   {
      hConf = sopen(pCfg,O_BINARY,SH_DENYNO);
      if(hConf == -1)
      {
         CCI_ret = CCIER_OPENFILE;
         return(mtFALSE);
      }
      len = lseek(hConf,0L,SEEK_END);
      if((len % sizeof(cfg_t)) != 0)
      {
         CCI_ret = CCIER_OPENFILE;
         close(hConf);
         return(mtFALSE);
      }
      lseek(hConf,0L,SEEK_SET);
      //
      // Initialisation des structures de gestion des cartes
      //
      for(ncarte=(int) len / sizeof(cfg_t),pcbp=CCI_pcbtab;ncarte;ncarte--,pcbp++)
      {
         err = read(hConf,&cfg,sizeof(cfg_t));
         if(err != sizeof(cfg_t))
         {
            CCI_ret = CCIER_READFILE;
            close(hConf);
            return(mtFALSE);
         }
         if((pcbp->p_segment = cfg.X25_Adr) == 0)
            continue;
         pcbp->p_segment <<= 4;
         pcbp->p_size      = 1024;
         //
         // Initialisation d'un pointeur en mode protg.
         //
         pcbp->p_xptr = (LPXPTR) _x386_mk_protected_ptr((unsigned long) pcbp->p_segment);
         if(! pcbp->p_xptr)
         {
            CCI_ret = CCIER_MKPTR;
            close(hConf);
            return(mtFALSE);
         }
         //
         // On mappe sur la mmoire physique, et on transforme en pointeur
         // near (void *).
         //
         ladr         = _x386_get_abs_address((void __far *) pcbp->p_xptr);
         pcbp->p_base = (mtPTRBYTE) _x386_map_physical_address((void *) ladr,pcbp->p_size);
         // Longueur utile	= longueur totale
         //                  - Entete requte
         //                  - 256 table d'alerte
         //                  - 14 octets info
         //                  - 2 octets registres (rcmd, rsta)
         //
         pcbp->p_sdat = pcbp->p_size - 256 - 16 - 8;
         //
         // Initialise les pointeurs d'accs … la mmoire partage.
         //
         pcbp->p_zalt = pcbp->p_base + pcbp->p_size - 256 - 16;
         pcbp->p_rcmd = pcbp->p_base + pcbp->p_size - 1;
         pcbp->p_rsta = pcbp->p_base + pcbp->p_size - 2;
         pcbp->p_zinf = (LPINFO) pcbp->p_base + pcbp->p_size - 16;
         //
         // Test prsence de la mmoire.
         //
         tmp = *pcbp->p_base;
         if((*pcbp->p_base = 0x55) != 0x55)
         {
            *pcbp->p_base = tmp;
            _x386_free_protected_ptr(pcbp->p_xptr);
            CCI_ret = CCIER_ERRMEM;
            close(hConf);
            return(mtFALSE);
         }
         *pcbp->p_base = tmp;
         //
         // Test prsence de la carte.
         //
         tmp = *pcbp->p_rcmd;
         // Envoi d'une commande invalide.
         *pcbp->p_rcmd = 0xFF;
         tm = time(NULL);
         // Timeout de 2 s.
         while((time(NULL) - tm) < 2)
         {
            // Attente d'un code de retour.
            // 0x80 = OK.
            // 0x81 = erreur.
            // 0xFF = Attendre la fin du traitement sur p_rsta.
            if(*pcbp->p_rcmd == 0x81)
               break;
         }
         if(*pcbp->p_rcmd != 0x81)
         {
            // Carte non prsente.
            *pcbp->p_rcmd = tmp;
            _x386_free_protected_ptr(pcbp->p_xptr);
            CCI_ret = CCIER_ERRDRV;
            close(hConf);
            return(mtFALSE);
         }
         *pcbp->p_rsta = 0x80;  // Acquittement de lecture.
         pcbp->p_flags = mtTRUE;
      }
      close(hConf);
      return(mtTRUE);
   }
   CCI_ret = CCIER_OPENFILE;
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dsinstallation des cartes.
//
void CCIEndCartes(void)
{
   int i;

   for(i=0;i<CCI_maxpcb;i++)
   {
      if(CCI_pcbtab[i].p_flags)
         _x386_free_protected_ptr(CCI_pcbtab[i].p_xptr);
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialisation de la zone de travail d'une fonction CCI...
//
static int InitZt(LPZT zt)
{
   // Teste l'erreur d'initialisation.
   if(CCI_ret)
      return(CCI_ret);
   // Vrification du numro de device.
   if(zt->z_dev > CCI_maxdev)
      return(-1);
   // Initialise la zone de travail.
   zt->z_noc = zt->z_dev / 256;
   zt->z_cnl = zt->z_dev & 0xFF;
   zt->z_pcb = &CCI_pcbtab[zt->z_noc];
   // Vrification de l'initialisation de la carte.
   if(!zt->z_pcb->p_flags)
      return(-1);
   // Vrification du numro de canal.
   if(zt->z_cnl > zt->z_pcb->p_zinf->inf_hst)
      return(-5);
   // Initialise les pointeurs pour la requte.
   zt->z_req.r = (LPREQ) zt->z_pcb->p_base;
   zt->z_dat   = zt->z_pcb->p_base + sizeof(req_t);
   return(0);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Envoi d'une requte … la carte.
//
static int SendReq(LPZT zp)
{
   time_t tm;
   mtBYTE result;

   zp->z_pcb->p_stat.cnt_req++;
   *zp->z_pcb->p_rcmd = 0;	/* IT -> carte, requete en debut de MP	*/
   //
   // Attente acquitement routine d'IT carte.
   //
   for(tm=time(NULL);(time(NULL) - tm) < 2;)
   {
      result = *zp->z_pcb->p_rcmd;
      if(result & 0x80)
         break;
   }
   // Vrification si la carte … rpondu.
   if(!(result & 0x80))
      return(-1);
   // Test si traitement diffr.
   if(result == 0xFF)
   {
      zp->z_pcb->p_stat.cnt_dif++;
      // Attente de l'acquitement de la tche.
      for(tm=time(NULL);(time(NULL) - tm) < 2;)
      {
         result = *zp->z_pcb->p_rsta;
         if(!(result & 0x80))
            break;
      }
      // Vrification si la carte … rpondu.
      if(result & 0x80)
         return(-2);
      // Acquitement pour la carte.
      *zp->z_pcb->p_rsta = 0x80;	/* IT -> carte, requete en debut de MP	*/
      // Prise du rsultat de la requte.
      result = zp->z_req.r->r_sta;
   }
   return(result & 0x7F);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//*-------------------------------------------------------------------------*
//:                    OUVERTURE D'UN CANAL DE DIALOGUE                     :
//*-------------------------------------------------------------------------*
//
//	DESCRIPTION
//		Cette fonction ouvre un canal de dialogue. Sur ouverture d'un canal,
//		la transmission et la reception sont rinitialises, et le service
//		d'accueil est dvalid.
//	
//	RETOUR
//		La fonction retourne 0 si elle s'est execute correctement, sinon elle
//		retourne un code d'erreur.
//		
int CCIopen(int dev)
{
   zt_t zt;
   int result;

   zt.z_dev = dev;
   if((result = InitZt(&zt)) != 0)
      return(result);
   zt.z_pcb->p_stat.cnt_open++;
   zt.z_req.r->r_siz = sizeof(req_t);
   zt.z_req.r->r_dev = (mtBYTE) zt.z_cnl;
   zt.z_req.r->r_fct = ROPEN;

   if((result = SendReq(&zt)) == CCI_NOERROR)
      zt.z_pcb->p_iosta[zt.z_cnl] = (mtBYTE) (zt.z_req.r->r_ios >> 8);

   return(result);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//*-------------------------------------------------------------------------*
// :                    FERMETURE D'UN CANAL DE DIALOGUE                     :
// *-------------------------------------------------------------------------*
// 
// 	DESCRIPTION
// 		La fonction CCIclose ferme un canal de dialogue. Si un buffer est
// 		pass … la fonction, le contenu de ce buffer sera envoy … chaque
// 		nouvelle connexion. On peut placer dans ce buffer un message d'accueil
// 		d'indisponibilit de l'application.
// 		Pour se faire, le canal doit tre initialis en mode confirmation
// 		automatique des appel (CAA=ON).
// 		
//	RETOUR
//		La fonction retourne 0 si elle s'est execute correctement, sinon elle
//		retourne un code d'erreur.
//		
int CCIclose(int dev, int cnt, mtPTRBYTE buf)
{
   zt_t zt;
   int result;

   zt.z_dev = dev;

   if((result = InitZt(&zt)) != 0)
      return(result);

   zt.z_pcb->p_stat.cnt_close++;

   zt.z_req.r->r_siz = sizeof(req_t);
   zt.z_req.r->r_dev = (mtBYTE) zt.z_cnl;
   zt.z_req.r->r_fct = RCLOSE;

   if(cnt)
   {
      if(cnt > zt.z_pcb->p_sdat)
         return(-3);
      memcpy(zt.z_dat,buf,cnt);
      zt.z_req.r->r_siz += (mtUSHORT) cnt;
   }

   return(SendReq(&zt));
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//*-------------------------------------------------------------------------*
// :                        LECTURE DU STATUS CANAL                          :
// *-------------------------------------------------------------------------*
//
//	DESCRIPTION
//		Cette fonction retourne dans sta le status du canal.
//	
//	RETOUR
//		La fonction retourne 0 si elle s'est execute correctement, sinon elle
//		retourne un code d'erreur.
//		
int CCIiosta(int dev, mtPTRWORD sta)
{
	zt_t	zt;
	int	result;

	zt.z_dev = dev;
	
	if( (result = InitZt(&zt)) != 0)
   	return(result);

	zt.z_pcb->p_stat.cnt_iosta++;

	if(zt.z_pcb->p_zalt[zt.z_cnl] & A_STACH)
   {
		zt.z_req.r->r_siz = sizeof(req_t);
		zt.z_req.r->r_dev = (mtBYTE) zt.z_cnl;
		zt.z_req.r->r_fct = RIOSTA;

		if((result = SendReq(&zt)) == CCI_NOERROR)
		{
			zt.z_pcb->p_iosta[zt.z_cnl] = (mtBYTE) (zt.z_req.r->r_ios >> 8);
			*sta = zt.z_req.r->r_ios;
			
			if(*sta & S_DLTCNX)
				zt.z_pcb->p_txsta[zt.z_cnl] = 0;
		}
	}
	else
   	*sta = zt.z_pcb->p_iosta[zt.z_cnl] << 8;

	return(result);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//*-------------------------------------------------------------------------*
// :                LECTURE-ECRITURE DES DONNEES DE CONTROLE                 :
// *-------------------------------------------------------------------------*
//
//	DESCRIPTION
//		Cette fonction permet d'envoyer et de reevoir des donnes de contr“le.
//	
//	RETOUR
//		Sur les 16 bits de poids faibles, la fonction retourne 0 si elle s'est
//		execute correctement, sinon elle retourne un code d'erreur.
//		Sur les 16 bits de poids forts, la fonction retourne la longueur du
//		buffer.
//		
int CCIioctl(int dev, int arg, mtPTRUSHORT cnt, mtPTRBYTE buf)
{
	zt_t	zt;
	int	result;

	zt.z_dev = dev;

	if( (result = InitZt(&zt)) != 0)
   	return(result);

	zt.z_pcb->p_stat.cnt_ioctl++;

	zt.z_req.r->r_siz = (mtUSHORT) *cnt + sizeof(req_t);
	zt.z_req.r->r_dev = (mtBYTE) zt.z_cnl;
	zt.z_req.r->r_fct = RIOCTL;
	zt.z_req.r->r_arg = (mtBYTE) arg;
	memcpy(zt.z_dat, buf, *cnt);

	if((result = SendReq(&zt)) == CCI_NOERROR)
	{
		*cnt = zt.z_req.r->r_siz - sizeof(req_t);
      memcpy(buf,zt.z_dat,*cnt);
	}

	return(result);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//*-------------------------------------------------------------------------*
//:                            ENVOI DE DONNEES                             :
//*-------------------------------------------------------------------------*
//
//	DESCRIPTION
//		Cette fonction permet d'envoyer des donnes sur un canal. La variable
//		arg dtermine la valeur des bit Q, M, et D.
//	
//	RETOUR
//		La fonction retourne 0 si elle s'est execute correctement, sinon elle
//		retourne un code d'erreur.
//		
int CCIoutput(int dev, int cnt, mtPTRBYTE buf, mtBYTE arg)
{
	zt_t	zt;
	int	result;
	int	count;

	zt.z_dev = dev;
	
	if( (result = InitZt(&zt)) != 0)
   	return(result);

	zt.z_pcb->p_stat.cnt_output++;

	zt.z_req.r->r_dev = (mtBYTE) zt.z_cnl;
	zt.z_req.r->r_fct = ROUTPUT;

	while(cnt)
	{
/*		if(cnt > zt.z_pcb->p_sdat)*/
		if(cnt > 512)
		{
			zt.z_req.r->r_arg = arg | ARG_DAS;
/*			count = zt.z_pcb->p_sdat;*/
			count = 512;
		}
		else
		{
			zt.z_req.r->r_arg = arg;
			count = cnt;
		}

  	   memcpy(zt.z_dat, buf, count);
		zt.z_req.r->r_siz = (mtUSHORT) count + sizeof(req_t);

		if((result = SendReq(&zt)) != CCI_NOERROR)
			break;

		buf += count;
		cnt -= count;
	}

	return(result);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//*-------------------------------------------------------------------------*
// :                   LECTURE DU STATUS DE TRANSMISSION                     :
// *-------------------------------------------------------------------------*
//
//	DESCRIPTION
//		Cette fonction retourne dans sta le status de transmission du canal,
//		et dans cnt, le compte de caractres pouvant tre envoys sans
//		attente.
//	
//	RETOUR
//		La fonction retourne 0 si elle s'est execute correctement, sinon elle
//		retourne un code d'erreur.
//		
int CCIoutput_sta(int dev, mtPTRUSHORT sta, mtPTRWORD cnt)
{
	zt_t	zt;
	int	result;

	zt.z_dev = dev;
	
	if( (result = InitZt(&zt)) != 0)
   	return(result);

	zt.z_pcb->p_stat.cnt_output_sta++;

	if(zt.z_pcb->p_zalt[zt.z_cnl] & (A_STACH | A_TXSTA))
   {
		zt.z_req.r->r_siz = sizeof(rio_t);
		zt.z_req.r->r_dev = (mtBYTE) zt.z_cnl;
		zt.z_req.r->r_fct = ROUTPUT_STA;

		if((result = SendReq(&zt)) == CCI_NOERROR)
		{
			zt.z_pcb->p_iosta[zt.z_cnl] = (mtBYTE) (zt.z_req.r->r_ios >> 8);
			zt.z_pcb->p_txsta[zt.z_cnl] = (mtBYTE) zt.z_req.r->r_ios;
			*sta = zt.z_req.r->r_ios;
			*cnt = zt.z_req.s->r_cnt;
		}
	}
	else
	{
		*sta = (zt.z_pcb->p_iosta[zt.z_cnl] << 8) | (zt.z_pcb->p_txsta[zt.z_cnl] & 0xf0);
		if(zt.z_pcb->p_zalt[zt.z_cnl] & A_TXEMP)
      {
			*sta |= S_EMPTY;
			*cnt = 512;
		}
		else
		{
			*sta |= S_FULL;
      	*cnt = 0;
      }
   }

	return(result);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//*-------------------------------------------------------------------------*
// :                  REINITIALISATION DE LA TRANSMISSION                    :
// *-------------------------------------------------------------------------*
//
//	DESCRIPTION
//		Cette fonction rinitialise la transmission. Toutes les donnes en
//		attente de transmission sont dtruites.
//	
//	RETOUR
//		La fonction retourne 0 si elle s'est execute correctement, sinon elle
//		retourne un code d'erreur.
//		
int CCIoutput_flh(int dev)
{
	zt_t	zt;
	int	result;

	zt.z_dev = dev;
	
	if( (result = InitZt(&zt)) != CCI_NOERROR)
		return(result);

	zt.z_pcb->p_stat.cnt_output_flh++;

	return(result);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//*-------------------------------------------------------------------------*
// :                     LECTURE DES DONNEES EN ATTENTE                      :
// *-------------------------------------------------------------------------*
//
//	DESCRIPTION
//		Cette fonction permet de lire les donnes en attente dans la carte.
//		cnt est la longueur maximum … recevoir. La fonction retourne dans arg
//		l'tat des bits Q, M, et D.
//	
//	RETOUR
//		La fonction retourne 0 sur les 16 bits de poids faible et la longueur
//		lu sur les 16 bits de poids forts si elle s'est execute correctement,
//		sinon elle retourne un code d'erreur.
//		
int CCIinput(int dev, mtPTRWORD cnt, mtPTRBYTE buf, mtPTRBYTE arg)
{
	zt_t	zt;
	int	result;

	zt.z_dev = dev;
	
	if( (result = InitZt(&zt)) != 0)
   	return(result);

	zt.z_pcb->p_stat.cnt_input++;

	zt.z_req.r->r_siz = (mtUSHORT) *cnt + sizeof(req_t);
	zt.z_req.r->r_dev = (mtBYTE) zt.z_cnl;
	zt.z_req.r->r_fct = RINPUT;

	if((result = SendReq(&zt)) == CCI_NOERROR)
	{
		*cnt = zt.z_req.r->r_siz - sizeof(req_t);
      memcpy(buf,zt.z_dat,*cnt);
		*arg = zt.z_req.r->r_arg;
	}

	return( result );
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//*-------------------------------------------------------------------------*
// :                     LECTURE DU STATUS DE RECEPTION                      :
// *-------------------------------------------------------------------------*
//
//	DESCRIPTION
//		Cette fonction retourne dans sta le status de reception du canal, et
//		dans cnt, le compte de caractres … lire.
//	
//	RETOUR
//		La fonction retourne 0 si elle s'est execute correctement, sinon elle
//		retourne un code d'erreur.
//		
int CCIinput_sta(int dev, mtPTRWORD sta, mtPTRWORD cnt)
{
	zt_t	zt;
	int	result;

	zt.z_dev = dev;
	
	if( (result = InitZt(&zt)) != 0)
   	return(result);

	zt.z_pcb->p_stat.cnt_input_sta++;

	if(zt.z_pcb->p_zalt[zt.z_cnl] & (A_STACH | A_RXRDY))
   {
		zt.z_req.r->r_siz = sizeof(rio_t);
		zt.z_req.r->r_dev = (mtBYTE) zt.z_cnl;
		zt.z_req.r->r_fct = RINPUT_STA;

		if((result = SendReq(&zt)) == 0)
		{
			zt.z_pcb->p_iosta[zt.z_cnl] = (mtBYTE) (zt.z_req.r->r_ios >> 8);
			*sta = zt.z_req.r->r_ios;
			*cnt = zt.z_req.s->r_cnt;
		}
	}
	else
   {
		*sta = (zt.z_pcb->p_iosta[zt.z_cnl] << 8) | S_EMPTY;
      *cnt = 0;
   }

	return(result);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//*-------------------------------------------------------------------------*
// :                    REINITIALISATION DE LA RECEPTION                     :
// *-------------------------------------------------------------------------*
//
//	DESCRIPTION
//		Cette fonction rinitialise la reception. Toutes les donnes en
//		attente de lecture sont dtruites.
//	
//	RETOUR
//		La fonction retourne 0 si elle s'est execute correctement, sinon elle
//		retourne un code d'erreur.
//		
int CCIinput_flh(int dev)
{
	zt_t	zt;
	int	result;

	zt.z_dev = dev;
	
	if( (result = InitZt(&zt)) != 0)
   	return(result);

	zt.z_pcb->p_stat.cnt_input_flh++;

	return(result);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//*-------------------------------------------------------------------------*
// :                 LECTURE DES INFORMATIONS SUR LA CARTE                   :
// *-------------------------------------------------------------------------*
//
int CCIinfo(int noc, LPINFO buf)
{
	zt_t	zt;
	int	result;

	zt.z_dev = noc * 256;
	
	if( (result = InitZt(&zt)) != 0)
   	return(result);
   	
	memcpy((void *) buf, (void *) zt.z_pcb->p_zinf, sizeof(info_t));
	return(0);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//*-------------------------------------------------------------------------*
// :                     LECTURE DES STATISTIQUES DRIVER                     :
// *-------------------------------------------------------------------------*
//
// DANS CCI.C = int CCIdrvstc(struct cci_stat *buf)
int CCIstat(int noc, LPSTAT buf, int flg_raz)
{
	zt_t	zt;
	int	result;

	zt.z_dev = noc * 256;
	
	if( (result = InitZt(&zt)) != 0)
   	return(result);
   	
	memcpy((void *) buf, (void *) &zt.z_pcb->p_stat, sizeof(stat_t));

   if(flg_raz)
	   memset((void *) &zt.z_pcb->p_stat, 0, sizeof(stat_t));

	return(0);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//

