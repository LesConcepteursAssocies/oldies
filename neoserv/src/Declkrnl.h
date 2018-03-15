// ��������������������������������͵NEOSERV����������������������������������͸
// �DECLKRNL.H   �                                                             �
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
// �Derni�re modification : 21/03/1996                                         �
// �                                                                           �
// �D�claration des primitives du kernel                                       �
// ���������������������������������������������������������������������������;

extern   mtPTRDICO  pCONSTTRUE;
extern   mtPTRDICO  pCONSTFALSE;
extern   mtPTRDICO  pCONSTLINE0;
extern   mtPTRDICO  pCONSTCOL1;
extern   mtPTRDICO  pCONSTFKENVOI;
extern   mtPTRDICO  pCONSTFKRETOUR;
extern   mtPTRDICO  pCONSTFKREPETITION;
extern   mtPTRDICO  pCONSTFKGUIDE;
extern   mtPTRDICO  pCONSTFKANNULATION;
extern   mtPTRDICO  pCONSTFKSOMMAIRE;
extern   mtPTRDICO  pCONSTFKCORRECTION;
extern   mtPTRDICO  pCONSTFKSUITE;
extern   mtPTRDICO  pCONSTBLACK;
extern   mtPTRDICO  pCONSTRED;
extern   mtPTRDICO  pCONSTGREEN;
extern   mtPTRDICO  pCONSTYELLOW;
extern   mtPTRDICO  pCONSTBLUE;
extern   mtPTRDICO  pCONSTMAGENTA;
extern   mtPTRDICO  pCONSTCYAN;
extern   mtPTRDICO  pCONSTWHITE;
extern   mtPTRDICO  pCONSTFSIMPLE;
extern   mtPTRDICO  pCONSTDHAUTEUR;
extern   mtPTRDICO  pCONSTDLARGEUR;
extern   mtPTRDICO  pCONSTDTAILLE;
extern   mtPTRDICO  pCONSTTMVDT;
extern   mtPTRDICO  pCONSTTMMXT;
extern   mtPTRDICO  pCONSTTMTEL;
extern   void       _DPush(mtPOINTEUR);
extern   mtPOINTEUR _DPop(void);
extern   void       _RPush(mtPOINTEUR);
extern   mtPOINTEUR _RPop(void);
extern   void       _2Points(void);
extern   void       _Next(void);
extern   void       _MNext(void);
extern   void       _OLNext(void);
extern   void       _GTNext(void);
extern   mtPTRDICO  DupliqueSym(mtPTRDICO);
extern   void       RecupSym(pPDICO,pPDICO,pPDICO,mtPTRBYTE,mtBOOL);
extern   float      RecupNumber(pPDICO,pPDICO,pPDICO,mtPTRBYTE,mtBOOL);
extern   mtPTRBYTE  RecupString(pPDICO,pPDICO,pPDICO,mtPTRBYTE,mtBOOL);
extern   void       _NStack(void);
extern   void       _Cls(void);
extern   void       _EnqRom(void);
extern   void       _SetColor(void);
extern   void       _SetBackGround(void);
extern   void       _GetCursor(void);
extern   void       _GetCursorPos(void);
extern   void       _Locate(void);
extern   void       _Ligne0(void);
extern   void       _Clear0(void);
extern   void       _SetFont(void);
extern   void       _Replicate(void);
extern   void       _SetTMode(void);
extern   void       _SendPage(void);
extern   void       _Emit(void);
extern   void       _Using(void);
extern   void       _NoUsing(void);
extern   void       _GetUsing(void);
extern   void       _Print(void);
extern   void       _PrintUsing(void);
extern   void       _Cr(void);
extern   void       _CursorOn(void);
extern   void       _CursorOff(void);
extern   void       _MBeep(void);
extern   void       _CligOn(void);
extern   void       _CligOff(void);
extern   void       _SouligOn(void);
extern   void       _SouligOff(void);
extern   void       _InverseOn(void);
extern   void       _InverseOff(void);
extern   void       _MUpper(void);
extern   void       _MLower(void);
extern   void       _MInput(void);
extern   void       _InitInputVar(void);
extern   void       _MInputVar(void);
extern   void       _InitMLInput(void);
extern   void       _MLInput(void);
extern   void       _PDup(void);
extern   void       _PSwap(void);
extern   void       __Over(void);
extern   void       _POver(void);
extern   void       _PRot(void);
extern   void       _PNip(void);
extern   void       _PDrop(void);
extern   void       _PPick(void);
extern   void       _LoopI(void);
extern   void       _LoopJ(void);
extern   void       _LoopK(void);
extern   void       __Egal(void);
extern   void       _SEgal(void);
extern   void       _SInf(void);
extern   void       _SSup(void);
extern   void       _SDiff(void);
extern   void       _SInfEgal(void);
extern   void       _SSupEgal(void);
extern   void       _LibCnx(void);
extern   void       _VFetch(void);
extern   void       _Space(void);
extern   void       _SpaceS(void);
extern   void       _IsEmpty(void);
extern   void       _DisplaySrv(void);
extern   void       _ExecuteProc(void);
extern   void       _ExecuteApp(void);
extern   void       _ChercheApp(void);
extern   void       _ExecKbdCmd(void);
extern   void       _Wait(void);
extern   void       _WaitOutBuffer(void);
extern   void       _InitWaitChar(void);
extern   void       _WaitChar(void);
extern   void       _InitDelay(void);
extern   void       _Delay(void);
extern   void       _VarStore(void);
extern   void       _SetGFKey(void);
extern   void       _SetGKeyW(void);
extern   void       _SetLFKey(void);
extern   void       _SetLKeyW(void);
extern   void       _DelLFKey(void);
extern   void       _DelGFKey(void);
extern   void       _DelLKeyW(void);
extern   void       _DelGKeyW(void);
extern   void       _DelUneGFKey(void);
extern   void       _DelUneLFKey(void);
extern   void       _DelUnGKeyW(void);
extern   void       _DelUnLKeyW(void);
extern   void       _DisLFKey(void);
extern   void       _DisGFKey(void);
extern   void       _DisLKeyW(void);
extern   void       _DisGKeyW(void);
extern   void       _EnaLFKey(void);
extern   void       _EnaGFKey(void);
extern   void       _EnaLKeyW(void);
extern   void       _EnaGKeyW(void);
extern   void       _DisTrAcc(void);
extern   void       _EnaTrAcc(void);
extern   void       _DisSeqEsc(void);
extern   void       _EnaSeqEsc(void);
extern   void       _EnaBinMode(void);
extern   void       _DisBinMode(void);
extern   void       _BAnd(void);
extern   void       _BOr(void);
extern   void       _BXor(void);
extern   void       _LAnd(void);
extern   void       _LOr(void);
extern   void       _LNot(void);
extern   void       _BComp1(void);
extern   void       _Inc1(void);
extern   void       _Dec1(void);
extern   void       _PlusEgal(void);
extern   void       _MoinsEgal(void);
extern   void       _MultEgal(void);
extern   void       _DivEgal(void);
extern   void       _ModEgal(void);
extern   void       _BAndEgal(void);
extern   void       _BOrEgal(void);
extern   void       _BXorEgal(void);
extern   void       _LeftShift(void);
extern   void       _RightShift(void);
extern   void       _LShiftEgal(void);
extern   void       _RShiftEgal(void);
extern   void       _Plus(void);
extern   void       _Moins(void);
extern   void       _Mult(void);
extern   void       _Div(void);
extern   void       _Mod(void);
extern   void       _ACos(void);
extern   void       _ASin(void);
extern   void       _ATan(void);
extern   void       _ATan2(void);
extern   void       _Ceil(void);
extern   void       _Cos(void);
extern   void       _Cosh(void);
extern   void       _Exp(void);
extern   void       _Expm1(void);
extern   void       _FAbs(void);
extern   void       _Negate(void);
extern   void       _Floor(void);
extern   void       _Hypot(void);
extern   void       _Log(void);
extern   void       _Log1p(void);
extern   void       _Log10(void);
extern   void       _Pow(void);
extern   void       _Pow10(void);
extern   void       _Rnd(void);
extern   void       _Randomize(void);
extern   void       _Sin(void);
extern   void       _Sinh(void);
extern   void       _Sqrt(void);
extern   void       _Tan(void);
extern   void       _Tanh(void);
extern   void       _FInt(void);
extern   void       _StrCat(void);
extern   void       _StrLen(void);
extern   void       _StrLower(void);
extern   void       _StrUpper(void);
extern   void       _StrRev(void);
extern   void       _StrAt(void);
extern   void       _StrSubstr(void);
extern   void       _StrVal(void);
extern   void       _StrNum(void);
extern   void       _StrRTrim(void);
extern   void       _StrLTrim(void);
extern   void       _StrAllTrim(void);
extern   void       _StrAscii(void);
extern   void       _StrChr(void);
extern   void       _StrSpn(void);
extern   void       _StrPbrk(void);
extern   void       _StrTok(void);
extern   void       _StrEncode(void);
extern   void       _ADir(void);
extern   void       _StrTran(void);
extern   void       _StrLeft(void);
extern   void       _StrRight(void);
extern   void       _StrPadL(void);
extern   void       _StrPadR(void);
extern   void       _StrPadC(void);
extern   void       _StrIns(void);
extern   void       _StrDel(void);
extern   void       _StrSet(void);
extern   void       _StrMatch(void);
extern   void       __TRUE(void);
extern   void       __FALSE(void);
extern   void       _PushTRUE(void);
extern   void       _PushFALSE(void);
extern   void       _SysFKEnvoi(void);
extern   void       _SysFKRetour(void);
extern   void       _SysFKRepetition(void);
extern   void       _SysFKGuide(void);
extern   void       _SysFKAnnulation(void);
extern   void       _SysFKSommaire(void);
extern   void       _SysFKCorrection(void);
extern   void       _SysFKSuite(void);
extern   void       _SysBlack(void);
extern   void       _SysRed(void);
extern   void       _SysGreen(void);
extern   void       _SysYellow(void);
extern   void       _SysBlue(void);
extern   void       _SysMagenta(void);
extern   void       _SysCyan(void);
extern   void       _SysWhite(void);
extern   void       _SysFSimple(void);
extern   void       _SysDHauteur(void);
extern   void       _SysDLargeur(void);
extern   void       _SysDTaille(void);
extern   void       _SysTModeVDT(void);
extern   void       _SysTModeMXT(void);
extern   void       _SysTModeTEL(void);
extern   void       _SysGetTMode(void);
extern   void       _SysMTouche(void);
extern   void       _SysMData(void);
extern   void       _SysTime(void);
extern   void       _SysTimer(void);
extern   void       _SysDate(void);
extern   void       _SysMTrameCNX(void);
extern   void       _SysDept(void);
extern   void       _SysPavi(void);
extern   void       _SysPalier(void);
extern   void       _SysNComp(void);
extern   void       _SysNApp(void);
extern   void       _SysCnxDate(void);
extern   void       _SysCnxTime(void);
extern   void       _SysMType(void);
extern   void       _SysNCV(void);
extern   void       _SysSrvDLanc(void);
extern   void       _SysSrvHLanc(void);
extern   void       _SysSrvMemAvail(void);
extern   void       _SysSrvChargeA(void);
extern   void       _SysSrvChargeM(void);
extern   void       _SysSrvNbCnx(void);
extern   void       _SysSrvPointeCnx(void);
extern   void       _SysSrvHFinServ(void);
extern   void       _DbOpen(void);
extern   void       _DbClose(void);
extern   void       _DbCloseAll(void);
extern   void       _DbAppendBlank(void);
extern   void       _DbBof(void);
extern   void       _DbBottom(void);
extern   void       _DbSelect(void);
extern   void       _DbSelected(void);
extern   void       _DbSetAlias(void);
extern   void       _DbDelete(void);
extern   void       _DbDeleted(void);
extern   void       _DbEof(void);
extern   void       _DbCommit(void);
extern   void       _DbCommitAll(void);
extern   void       _DbGo(void);
extern   void       _DbLock(void);
extern   void       _DbLocked(void);
extern   void       _DbFLock(void);
extern   void       _DbFLocked(void);
extern   void       _DbIdxName(void);
extern   void       _DbIdxKey(void);
extern   void       _DbRecall(void);
extern   void       _DbReccount(void);
extern   void       _DbRecno(void);
extern   void       _DbSeek(void);
extern   void       _DbSkip(void);
extern   void       _DbSetIndex(void);
extern   void       _DbTop(void);
extern   void       _DbUnLock(void);
extern   void       _DbUnLockAll(void);
extern   void       _DbReplace(void);
extern   void       _DbField(void);
extern   void       _DbFieldType(void);
extern   void       _DbFieldLen(void);
extern   void       _DbFieldCount(void);
extern   void       _DbFieldLenDec(void);
extern   void       _DbFieldNumToName(void);
extern   void       _DbError(void);
extern   void       _IntError(void);
extern   void       _ErrorCode(void);
extern   void       _ALen(void);
extern   void       _InitAfill(void);
extern   void       _AFill(void);
extern   void       _InitAsearch(void);
extern   void       _ASearch(void);
extern   void       _AIns(void);
extern   void       _ADel(void);
extern   void       _TransTab(void);
extern   void       _ACopy(void);
extern   void       _CopyTab(void);
extern   void       _AClone(void);
extern   void       _DeleteTab(void);
extern   void       _DecoupeChaine(void);
extern   void       __CountADirFiles(void);
extern   void       _ADirTab(void);
extern   void       _SetLTask(void);
extern   void       _DeleteLTask(void);
extern   void       _IsLTaskActive(void);
extern   void       _DateDtoS(void);
extern   void       _DateStoD(void);
extern   void       _DateDtoJ(void);
extern   void       _DateJtoD(void);
extern   void       _DateDay(void);
extern   void       _DateDow(void);
extern   void       _DateMonth(void);
extern   void       _DateYear(void);
extern   void       _DateCDow(void);
extern   void       _DateCMonth(void);
extern   void       _FTOpen(void);
extern   void       _FTClose(void);
extern   void       _FTEof(void);
extern   void       _FTReadLn(void);
extern   void       _FTRead(void);
extern   void       _FTSize(void);
extern   void       _CRC16Buff(void);
extern   void       _CRC32Buff(void);
extern   void       _Conv8to7(void);
extern   void       _Conv7to8(void);
extern   void       __XXEncode(void);
extern   void       __XXDecode(void);
extern   void       __UUEncode(void);
extern   void       __UUDecode(void);
extern   void       _FlushOutBuff(void);
extern   void       _TransmitBuff(void);
//
// ����������������������������������������������������������������������������
//
