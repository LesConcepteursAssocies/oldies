// ��������������������������������͵NEOSERV����������������������������������͸
// �MINITEL.H    �                                                             �
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
// �Derni�re modification : 31/07/1995                                         �
// �                                                                           �
// �D�finitions des constantes relatives au minitel                            �
// ���������������������������������������������������������������������������;

#if !defined(__MINITEL_H)
#define _MINITEL_H

#define  mtCR           0x0D
#define  mtLF           0x0A
#define  MSep           0x13
#define  mtUS           0x1F

#define  MEnvoi         0x41
#define  MRetour        0x42
#define  MRepetition    0x43
#define  MGuide         0x44
#define  MAnnulation    0x45
#define  MSommaire      0x46
#define  MCorrection    0x47
#define  MSuite         0x48
#define  MCnxFin        0x49

#define  MEtoile        0x2A
#define  MDiese         0x23

#define  PRO1           "\x1B\x39"
#define  PRO2           "\x1B\x3A"
#define  PRO3           "\x1B\x3B"
#define  CSI            "\x1B\x5B"

#endif
//
// --------------------------------------------------------------------------
//

