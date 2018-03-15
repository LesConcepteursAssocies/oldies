/*
** ----------------------------------------------------------------------------
** Projet gameDream
**
** Auteur               : Guillaumaud Philippe
**
** commencé le          : 01/04/1999
** dernière modification: 22/02/2000
**
** GBtrackball.h
** ----------------------------------------------------------------------------
*/
#ifndef _GBTRACKBALL_H
#define _GBTRACKBALL_H

void trackball(float q[4], float p1x, float p1y, float p2x, float p2y);
void axis_to_quat(float a[3], float phi, float q[4]);
void add_quats(float *q1, float *q2, float *dest);
void build_rotmatrix(float m[4][4], float q[4]);

#endif
/*
** ----------------------------------------------------------------------------
*/
