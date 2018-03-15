// ��������������������������������͵NEOSERV����������������������������������͸
// �PDATA.H      �                                                             �
// ���������������                                                             �
// �Auteur                : Guillaumaud Philippe                               �
// �                        teknik@yaka2.com                                   �
// �                        http://www.yaka2.com/teknik                        �
// �                                                                           �
// �Vous �tes autoris� � diffuser et � utiliser ce logiciel sans restrictions. �
// �                                                                           �
// �Ce logiciel est fourni tel quel, sans aucune garantie.                     �
// �L'auteur ne saurait voir sa responsabilit� engag�e en cas de               �
// �dommages de quelque nature que ce soit subis par l'utilisateur             �
// �ou des tiers et r�sultant directement ou                                   �
// �indirectement de son utilisation, notamment la perte de donn�es,           �
// �ou toute perte financi�re r�sultant de son                                 �
// �utilisation ou de l'impossibilit� de l'utiliser, et ceci m�me si           �
// �l'auteur a �t� pr�venu de la possibilit� de tels dommages.                 �
// �                                                                           �
// �Commenc� le           : 09/06/1993                                         �
// �Derni�re modification : 30/09/1995                                         �
// �                                                                           �
// �D�finitions des donn�es relatives aux Piles de paquets                     �
// ���������������������������������������������������������������������������;

#if !defined(__PDATA_H)
#define _PDATA_H

//
// Structure de type pile de paquets
//
typedef struct _PData
{
   mtUWORD          NumCv;             // N� de CV propri�taire du paquet.
   int              iPort;             // N� de port de r�ception.
   mtWORD           wLen;              // Longueur du paquet.
   mtPTRBYTE        pData;             // Pointe sur le paquet.
   mtBYTE           Type;              // Type du paquet.
} PData;

typedef PData * mtPTRPDATA;
typedef PData ** pPDATA;

#define  SDATAPAGE      1024           // Taille d'une page de pile.

#endif

