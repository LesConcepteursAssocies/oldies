// ÕÍÍÍÍÍÍÍÍÍÍÍÍÍÑÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍµNEOSERVÆÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¸
// ³MINITEL.H    ³                                                             ³
// ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÙ                                                             ³
// ³Auteur                : Guillaumaud Philippe                               ³
// ³                        teknik@yaka2.com                                   ³
// ³                        http://www.yaka2.com/teknik                        ³
// ³                                                                           ³
// ³Vous ˆtes autoris‚ … diffuser et … utiliser ce logiciel sans restrictions. ³
// ³                                                                           ³
// ³Ce logiciel est fourni tel quel, sans aucune garantie.                     ³
// ³L'auteur ne saurait voir sa responsabilit‚ engag‚e en cas de               ³
// ³dommages de quelque nature que ce soit subis par l'utilisateur             ³
// ³ou des tiers et r‚sultant directement ou                                   ³
// ³indirectement de son utilisation, notamment la perte de donn‚es,           ³
// ³ou toute perte financiŠre r‚sultant de son                                 ³
// ³utilisation ou de l'impossibilit‚ de l'utiliser, et ceci mˆme si           ³
// ³l'auteur a ‚t‚ pr‚venu de la possibilit‚ de tels dommages.                 ³
// ³                                                                           ³
// ³Commenc‚ le           : 09/06/1993                                         ³
// ³DerniŠre modification : 31/07/1995                                         ³
// ³                                                                           ³
// ³D‚finitions des constantes relatives au minitel                            ³
// ÔÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¾

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

