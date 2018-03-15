// ีอออออออออออออัออออออออออออออออออตNEOSERVฦออออออออออออออออออออออออออออออออออธ
// ณCONVBUFF.C   ณ                                                             ณ
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
// ณDernire modification : 26/10/1995                                         ณ
// ณ                                                                           ณ
// ณConversions 7 bits - 8 bits.                                               ณ
// ิอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ

#include "generic.h"

#define  mapEOF         -1
#define  BIT6MASK       0x3F
#define  INLEN          45
#define  OUTLEN         60
#define  UNDEF_CH       -2
//
// Table d'encodage UU
//
static char sUUtable[] = "`!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_";
static int  iUUlen     = 'M';
static int  aiUUmap[257];
static int  *piUUmap   = aiUUmap + 1;
//
// Table d'encodage XX
//
static char sXXtable[] = "+-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
static int  iXXlen     = 'h';
static int  aiXXmap[257];
static int  *piXXmap   = aiXXmap + 1;

//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Translation 8bits->7Bits
//
void tr8to7(mtBYTE **src, mtBYTE **dst)
{
   if((**src>=0xD1 && **src<=0xFF) || (**src>=0x00 && **src<=0x1F))
   {
      **dst     = 0x7E;
      *(++*dst) = **src+0x50;
      ++*dst;
      ++*src;
   }
   else
   {
      if(**src>=0x7B && **src<=0xD0)
      {
         **dst=0x7B;
         *(++*dst)=**src-0x58;
         ++*dst;
         ++*src;
      }
      else
         *((*dst)++)=*((*src)++);
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Translation 7bits->8bits
//
void tr7to8(mtBYTE **src, mtBYTE **dst)
{
   switch(**src)
   {
      case 0x7E: { **dst=*(++*src)-0x50; break; }
      case 0x7B: { **dst=*(++*src)+0x58; break; }
      default  : { **dst=**src;          break; }
   }
   ++*dst;
   ++*src;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialisation de l'encodage XX
//
void SetXXEncode(void)
{
   int i;

   for (i = 0;i <= 255;i++)
      piXXmap[i] = UNDEF_CH;
   for (i = 0;i < 64;i++)
      piXXmap[sXXtable[i]] = i;
   piXXmap[mapEOF] = mapEOF;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialisation de l'encodage UU
//
void SetUUEncode(void)
{
   int i;
   for (i = 0;i <= 255;i++)
      piUUmap[i] = UNDEF_CH;
   for (i = 0;i < 64;i++)
      piUUmap[sUUtable[i]] = i;
   piUUmap[mapEOF] = mapEOF;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Encodage d'un buffer.
//
void _Encode(mtPTRBYTE ibuf,mtWORD iblen,mtPTRBYTE obuf,mtPTRBYTE sTable)
{
   int i,j,len,nbytes3;

   do
   {
      if(iblen > INLEN)
      {
         nbytes3 = INLEN;
         iblen -= INLEN;
      }
      else
      {
         nbytes3 = iblen ;
         iblen = 0;
      }
      if (nbytes3 == INLEN)
         len = OUTLEN;
      else
      {
         nbytes3 = (nbytes3 + 2) / 3;
         len = 4 * nbytes3;
         nbytes3 *= 3;
      }

      for (i = nbytes3 - 3, j = len - 4; i >= 0; i -= 3, j -= 4)/* encode */
      {
         obuf[j + 3] = sTable[ibuf[i + 2] & BIT6MASK];
         obuf[j + 2] = sTable[((ibuf[i+1] << 2) + (ibuf[i+2] >> 6)) & BIT6MASK];
         obuf[j + 1] = sTable[((ibuf[i] << 4)   + (ibuf[i+1] >> 4)) & BIT6MASK];
         obuf[j]     = sTable[(ibuf[i] >> 2) & BIT6MASK];
      }
   } while (nbytes3 > 0 );
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dcode un caractre du buffer donn.
//
int _recover(mtWORD i, mtWORD nbytes3, mtWORD len,mtPTRBYTE ibuf,mtPTRBYTE sTable,mtPTRWORD pMap,mtWORD iNNlen)
{
   int j,ch;

   do
   {
      if(i >= len)
         // fin du dcodage.
         return(sTable[0]);
      if(pMap[ibuf[i]] == mapEOF)
         // Fin de ligne rencontre.
         return(sTable[0]);                        /* return null char */
      if(ibuf[i] == '\n')
      {
         if(nbytes3 == iNNlen)
            return(mapEOF);
         else
            if(nbytes3 < iNNlen)
               return(sTable[0]);
            else
               // Ligne corrompue.
               return(mapEOF);
      }
      else
         if(ibuf[i] >= ' ')
            // Caractre illgal.
            return(mapEOF);
         else
            return(mapEOF);

      for(j = i;j < len - 1;j++)
         ibuf[j] = ibuf[j + 1];
      ibuf[j] = sTable[0];
   } while (pMap[ibuf[i]] < 0);
   return(pMap[ibuf[i]]);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dcodage du buffer donn
//
void _Decode(mtPTRBYTE ibuf,mtWORD iblen,mtPTRBYTE obuf,mtPTRBYTE sTable,mtPTRWORD pMap,mtWORD iNNlen)
{
   int i,j,len,k1,k2,k3,k4,nbytes3;

   nbytes3 = pMap[iNNlen];
   len = (nbytes3 * 4) / 3 + (nbytes3 % 3 > 0);
   for(i = j = 0;i < iblen;i += 3, j += 4)
   {
      if((k1 = pMap[ibuf[j]]) < 0)
         k1 = _recover(j,nbytes3,len,ibuf,sTable,pMap,iNNlen);
      if((k2 = pMap[ibuf[j + 1]]) < 0)
         k2 = _recover(j + 1,nbytes3,len,ibuf,sTable,pMap,iNNlen);
      if((k3 = pMap[ibuf[j + 2]]) < 0)
         k3 = _recover(j + 2,nbytes3,len,ibuf,sTable,pMap,iNNlen);
      if((k4 = pMap[ibuf[j + 3]]) < 0)
         k4 = _recover(j + 3, nbytes3, len,ibuf,sTable,pMap,iNNlen);
      obuf[i]     = (k1 << 2) + (k2 >> 4);
      obuf[i + 1] = (k2 << 4) + (k3 >> 2);
      obuf[i + 2] = (k3 << 6) +  k4;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Encodage XX.
//
void _XXEncode(mtPTRBYTE ibuf,mtWORD iblen,mtPTRBYTE obuf)
{
   _Encode(ibuf,iblen,obuf,sXXtable);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dcodage XX.
//
void _XXDecode(mtPTRBYTE ibuf,mtWORD iblen,mtPTRBYTE obuf)
{
   _Decode(ibuf,iblen,obuf,sXXtable,piXXmap,piXXmap[iXXlen]);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Encodage UU.
//
void _UUEncode(mtPTRBYTE ibuf,mtWORD iblen,mtPTRBYTE obuf)
{
   _Encode(ibuf,iblen,obuf,sUUtable);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dcodage UU.
//
void _UUDecode(mtPTRBYTE ibuf,mtWORD iblen,mtPTRBYTE obuf)
{
   _Decode(ibuf,iblen,obuf,sUUtable,piUUmap,piUUmap[iUUlen]);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//

