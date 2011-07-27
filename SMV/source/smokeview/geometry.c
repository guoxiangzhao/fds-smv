// $Date$ 
// $Revision$
// $Author$

#include "options.h"
#ifdef pp_GPU
#include "glew.h"
#endif
#include <stdio.h>  
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef pp_OSX
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "contourdefs.h"
#include "isodefs.h"

#include "flowfiles.h"
#include "smokeviewapi.h"
#include "MALLOC.h"
#include "smokeviewvars.h"


// svn revision character string
char geometry_revision[]="$Revision$";


/* ------------------ ExtractFrustum ------------------------ */

void ExtractFrustum(void){

/* code from:  http://www.crownandcutlass.com/features/technicaldetails/frustum.html */
   float   proj[16];
   float   modl[16];
   float   clip[16];
   float   t;

   /* Get the current PROJECTION matrix from OpenGL */
   glGetFloatv( GL_PROJECTION_MATRIX, proj );

   /* Get the current MODELVIEW matrix from OpenGL */
   glGetFloatv( GL_MODELVIEW_MATRIX, modl );

   /* Combine the two matrices (multiply projection by modelview) */
   clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
   clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
   clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
   clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];

   clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
   clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
   clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
   clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];

   clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
   clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
   clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
   clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];

   clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
   clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
   clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
   clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];

   /* Extract the numbers for the RIGHT plane */
   frustum[0][0] = clip[ 3] - clip[ 0];
   frustum[0][1] = clip[ 7] - clip[ 4];
   frustum[0][2] = clip[11] - clip[ 8];
   frustum[0][3] = clip[15] - clip[12];

   /* Normalize the result */
   t = sqrt( frustum[0][0] * frustum[0][0] + frustum[0][1] * frustum[0][1] + frustum[0][2] * frustum[0][2] );
   frustum[0][0] /= t;
   frustum[0][1] /= t;
   frustum[0][2] /= t;
   frustum[0][3] /= t;

   /* Extract the numbers for the LEFT plane */
   frustum[1][0] = clip[ 3] + clip[ 0];
   frustum[1][1] = clip[ 7] + clip[ 4];
   frustum[1][2] = clip[11] + clip[ 8];
   frustum[1][3] = clip[15] + clip[12];

   /* Normalize the result */
   t = sqrt( frustum[1][0] * frustum[1][0] + frustum[1][1] * frustum[1][1] + frustum[1][2] * frustum[1][2] );
   frustum[1][0] /= t;
   frustum[1][1] /= t;
   frustum[1][2] /= t;
   frustum[1][3] /= t;

   /* Extract the BOTTOM plane */
   frustum[2][0] = clip[ 3] + clip[ 1];
   frustum[2][1] = clip[ 7] + clip[ 5];
   frustum[2][2] = clip[11] + clip[ 9];
   frustum[2][3] = clip[15] + clip[13];

   /* Normalize the result */
   t = sqrt( frustum[2][0] * frustum[2][0] + frustum[2][1] * frustum[2][1] + frustum[2][2] * frustum[2][2] );
   frustum[2][0] /= t;
   frustum[2][1] /= t;
   frustum[2][2] /= t;
   frustum[2][3] /= t;

   /* Extract the TOP plane */
   frustum[3][0] = clip[ 3] - clip[ 1];
   frustum[3][1] = clip[ 7] - clip[ 5];
   frustum[3][2] = clip[11] - clip[ 9];
   frustum[3][3] = clip[15] - clip[13];

   /* Normalize the result */
   t = sqrt( frustum[3][0] * frustum[3][0] + frustum[3][1] * frustum[3][1] + frustum[3][2] * frustum[3][2] );
   frustum[3][0] /= t;
   frustum[3][1] /= t;
   frustum[3][2] /= t;
   frustum[3][3] /= t;

   /* Extract the FAR plane */
   frustum[4][0] = clip[ 3] - clip[ 2];
   frustum[4][1] = clip[ 7] - clip[ 6];
   frustum[4][2] = clip[11] - clip[10];
   frustum[4][3] = clip[15] - clip[14];

   /* Normalize the result */
   t = sqrt( frustum[4][0] * frustum[4][0] + frustum[4][1] * frustum[4][1] + frustum[4][2] * frustum[4][2] );
   frustum[4][0] /= t;
   frustum[4][1] /= t;
   frustum[4][2] /= t;
   frustum[4][3] /= t;

   /* Extract the NEAR plane */
   frustum[5][0] = clip[ 3] + clip[ 2];
   frustum[5][1] = clip[ 7] + clip[ 6];
   frustum[5][2] = clip[11] + clip[10];
   frustum[5][3] = clip[15] + clip[14];

   /* Normalize the result */
   t = sqrt( frustum[5][0] * frustum[5][0] + frustum[5][1] * frustum[5][1] + frustum[5][2] * frustum[5][2] );
   frustum[5][0] /= t;
   frustum[5][1] /= t;
   frustum[5][2] /= t;
   frustum[5][3] /= t;
}

/* ------------------ PointInFrustum ------------------------ */

int PointInFrustum( float x, float y, float z){
   int p;

   if( frustum[0][0]*x + frustum[0][1]*y + frustum[0][2]*z + frustum[0][3] <= 0 )return 0;
   if( frustum[1][0]*x + frustum[1][1]*y + frustum[1][2]*z + frustum[1][3] <= 0 )return 0;
   if( frustum[2][0]*x + frustum[2][1]*y + frustum[2][2]*z + frustum[2][3] <= 0 )return 0;
   if( frustum[3][0]*x + frustum[3][1]*y + frustum[3][2]*z + frustum[3][3] <= 0 )return 0;
   if( frustum[4][0]*x + frustum[4][1]*y + frustum[4][2]*z + frustum[4][3] <= 0 )return 0;
   if( frustum[5][0]*x + frustum[5][1]*y + frustum[5][2]*z + frustum[5][3] <= 0 )return 0;
   return 1;
}

/* ------------------ RectangleInFrustum ------------------------ */

int RectangleInFrustum( float *x11, float *x12, float *x22, float *x21){
   int p;

   for( p = 0; p < 6; p++ ){
      if( frustum[p][0]*x11[0] + frustum[p][1]*x11[1] + frustum[p][2]*x11[2] + frustum[p][3] > 0 )continue;
      if( frustum[p][0]*x12[0] + frustum[p][1]*x12[1] + frustum[p][2]*x12[2] + frustum[p][3] > 0 )continue;
      if( frustum[p][0]*x22[0] + frustum[p][1]*x22[1] + frustum[p][2]*x22[2] + frustum[p][3] > 0 )continue;
      if( frustum[p][0]*x21[0] + frustum[p][1]*x21[1] + frustum[p][2]*x21[2] + frustum[p][3] > 0 )continue;
      return 0;
   }
   return 1;
}


/* ------------------ matmatmult ------------------------ */

void matmatmult(float *m1, float *m2, float *m3){
  int i, j, k;
  int ij;

  for(i=0;i<4;i++){
    for(j=0;j<4;j++){
      ij = i+4*j;
      m3[ij]=0.0;
      for(k=0;k<4;k++){
        m3[ij]+=m1[i+4*k]*m2[k+4*j];
      }
    }
  }
}

/* ------------------ getinverse ------------------------ */

void getinverse(float *m, float *mi){
  int i,j;
  float *v,*vi;

  /*

  assume m is a 4x4 matrix parttioned as

  q00 q01 q02 v0
  q10 q11 q12 v1
  q20 q21 q22 v2
    0   0   0  a

  where v=(vi) and Q=(qij) is orthogonal ( Q*transpose(Q) = I )

  then inverse(m) =     transpose(Q)   -transpose(Q)*v/a
                            0                 1/a       

  note:  m_ij = m[i+4*j]
  */

  v=m+12;   /* fourth column of m */               
  vi=mi+12; /* fourth column of inverse(m) */
  for(i=0;i<3;i++){  /* compute transpose */
    for(j=0;j<3;j++){
      mi[i+4*j]=m[j+4*i];
    }
    mi[3+4*j]=0.0;
  }
  vi[3]=1.0/v[3];
  vi[0]=-(mi[0+4*0]*v[0]+mi[0+4*1]*v[1]+mi[0+4*2]*v[2])*vi[3];
  vi[1]=-(mi[1+4*0]*v[0]+mi[1+4*1]*v[1]+mi[1+4*2]*v[2])*vi[3];
  vi[2]=-(mi[2+4*0]*v[0]+mi[2+4*1]*v[1]+mi[2+4*2]*v[2])*vi[3];
}

/* ------------------ compareisonodes ------------------------ */

int compare_volfacelistdata( const void *arg1, const void *arg2 ){
  volfacelistdata *vi, *vj;

  vi = *(volfacelistdata **)arg1;
  vj = *(volfacelistdata **)arg2;

  if(vi->dist2<vj->dist2)return 1;
  if(vi->dist2>vj->dist2)return -1;
  return 0;
}

/* ------------------ getvolsmokedir ------------------------ */

void getvolsmokedir(float *mm){
    /*
      ( m0 m4 m8  m12 ) (x)    (0)
      ( m1 m5 m9  m13 ) (y)    (0)
      ( m2 m6 m10 m14 ) (z)  = (0)
      ( m3 m7 m11 m15 ) (1)    (1)

       ( m0 m4  m8 )      (m12)
   Q=  ( m1 m5  m9 )  u = (m13)
       ( m2 m6 m10 )      (m14)
      
      (Q   u) (x)     (0)      
      (v^T 1) (y)   = (1)
       
      m3=m7=m11=0, v^T=0, y=1   Qx+u=0 => x=-Q^Tu
    */
  int i,ii,j;
  float norm[3];
  float pi;
  float eyedir[3];
  float cosdir;
  float angles[7];

  volfacelistdata *vi;

  pi=4.0*atan(1.0);

  xyzeyeorig[0] = -(mm[0]*mm[12]+mm[1]*mm[13]+ mm[2]*mm[14])/mscale[0];
  xyzeyeorig[1] = -(mm[4]*mm[12]+mm[5]*mm[13]+ mm[6]*mm[14])/mscale[1];
  xyzeyeorig[2] = -(mm[8]*mm[12]+mm[9]*mm[13]+mm[10]*mm[14])/mscale[2];
  
  for(j=0;j<nmeshes;j++){
    mesh *meshj;
    
    meshj = meshinfo + j;

    meshj->inside=0;
    if(
      xyzeyeorig[0]>meshj->x0&&xyzeyeorig[0]<meshj->x1&&
      xyzeyeorig[1]>meshj->y0&&xyzeyeorig[1]<meshj->y1&&
      xyzeyeorig[2]>meshj->z0&&xyzeyeorig[2]<meshj->z1
      ){
      for(i=-3;i<=3;i++){
        if(i==0)continue;
        meshj->drawsides[i+3]=1;
      }
      meshj->inside=1;
      continue;
    }

    for(i=-3;i<=3;i++){
      if(i==0)continue;
      ii = i;
      if(i<0)ii=-i;
      norm[0]=0.0;
      norm[1]=0.0;
      norm[2]=0.0;
      switch (ii){
      case 1:
        if(i<0){
          norm[0]=-1.0;
          eyedir[0]=meshj->x0;
        }
        else{
          norm[0]=1.0;
          eyedir[0]=meshj->x1;
        }
        eyedir[1]=meshj->ycen;
        eyedir[2]=meshj->zcen;
        break;
      case 2:
        eyedir[0]=meshj->xcen;
        if(i<0){
          norm[1]=-1.0;
          eyedir[1]=meshj->y0;
        }
        else{
          norm[1]=1.0;
          eyedir[1]=meshj->y1;
        }
        eyedir[2]=meshj->zcen;
        break;
      case 3:
        eyedir[0]=meshj->xcen;
        eyedir[1]=meshj->ycen;
        if(i<0){
          norm[2]=-1.0;
          eyedir[2]=meshj->z0;
        }
        else{
          norm[2]=1.0;
          eyedir[2]=meshj->z1;
        }
        break;
      default:
        ASSERT(FFALSE);
        break;
      }
      eyedir[0]=xyzeyeorig[0]-eyedir[0];
      eyedir[1]=xyzeyeorig[1]-eyedir[1];
      eyedir[2]=xyzeyeorig[2]-eyedir[2];
      normalize(eyedir,3);
      cosdir = (eyedir[0]*norm[0]+eyedir[1]*norm[1]+eyedir[2]*norm[2]);
      if(cosdir>1.0)cosdir=1.0;
      if(cosdir<-1.0)cosdir=-1.0;
      cosdir=acos(cosdir)*180.0/pi;
      if(cosdir<0.0)cosdir=-cosdir;
      angles[3+i]=cosdir;
    }
    for(i=-3;i<=3;i++){
      if(i==0)continue;
      if(angles[i+3]<90.0){
        meshj->drawsides[i+3]=1;
      }
      else{
        meshj->drawsides[i+3]=0;
      }
    }
  }

  vi = volfacelistinfo;
  nvolfacelistinfo=0;
  for(i=0;i<nmeshes;i++){
    mesh *meshi;
    int facemap[7]={12,6,0,0,3,9,15};
    volrenderdata *vr;

    meshi = meshinfo + i;
    vr = &(meshi->volrenderinfo);
    if(vr->firedataptr==NULL&&vr->smokedataptr==NULL)continue;
    if(vr->loaded==0||vr->display==0)continue;
    for(j=-3;j<=3;j++){
      float dx, dy, dz;
      float *xyz;

      if(j==0)continue;
      if(meshi->drawsides[j+3]==0)continue;
      vi->facemesh=meshi;
      vi->iwall=j;
      xyz=meshi->face_centers+facemap[j+3];
      dx = xyz[0]-xyzeyeorig[0];
      dy = xyz[1]-xyzeyeorig[1];
      dz = xyz[2]-xyzeyeorig[2];
      vi->dist2=dx*dx+dy*dy+dz*dz;
      vi->xyz=xyz;
      vi++;
      nvolfacelistinfo++;
    }
  }
  if(nvolfacelistinfo>0){
    for(i=0;i<nvolfacelistinfo;i++){
      volfacelistinfoptrs[i]=volfacelistinfo + i;
    }
    qsort((volfacelistdata *)volfacelistinfoptrs,nvolfacelistinfo,sizeof(volfacelistdata *),compare_volfacelistdata);
  }
  printf("sorted vr's\n");//xyz

}

/* ------------------ getsmokedir ------------------------ */

void getsmokedir(float *mm){
    /*
      ( m0 m4 m8  m12 ) (x)    (0)
      ( m1 m5 m9  m13 ) (y)    (0)
      ( m2 m6 m10 m14 ) (z)  = (0)
      ( m3 m7 m11 m15 ) (1)    (1)

       ( m0 m4  m8 )      (m12)
   Q=  ( m1 m5  m9 )  u = (m13)
       ( m2 m6 m10 )      (m14)
      
      (Q   u) (x)     (0)      
      (v^T 1) (y)   = (1)
       
      m3=m7=m11=0, v^T=0, y=1   Qx+u=0 => x=-Q^Tu
    */
  int i,ii,j;
  mesh *meshj;
  float norm[3],scalednorm[3];
  float normdir[3];
  float absangle,cosangle,minangle;
  int iminangle;
  float dx, dy, dz;
  float factor;
  float pi;

  pi=4.0*atan(1.0);

  xyzeyeorig[0] = -(mm[0]*mm[12]+mm[1]*mm[13]+ mm[2]*mm[14])/mscale[0];
  xyzeyeorig[1] = -(mm[4]*mm[12]+mm[5]*mm[13]+ mm[6]*mm[14])/mscale[1];
  xyzeyeorig[2] = -(mm[8]*mm[12]+mm[9]*mm[13]+mm[10]*mm[14])/mscale[2];
  
  for(j=0;j<nmeshes;j++){
    meshj = meshinfo + j;

    minangle=1000.0;
    iminangle=-10;
    meshj->dx=meshj->xplt_orig[1]-meshj->xplt_orig[0];
    meshj->dy=meshj->yplt_orig[1]-meshj->yplt_orig[0];
    meshj->dz=meshj->zplt_orig[1]-meshj->zplt_orig[0];
    meshj->dxy=meshj->dx*meshj->dx+meshj->dy*meshj->dy;
    meshj->dxy=sqrt(meshj->dxy)/2.0;
    meshj->dxz=meshj->dx*meshj->dx+meshj->dz*meshj->dz;
    meshj->dxz=sqrt(meshj->dxz)/2.0;
    meshj->dyz=meshj->dy*meshj->dy+meshj->dz*meshj->dz;
    meshj->dyz=sqrt(meshj->dyz)/2.0;



    meshj->dy/=meshj->dx;
    meshj->dz/=meshj->dx;
    meshj->dxy/=meshj->dx;
    meshj->dxz/=meshj->dx;
    meshj->dyz/=meshj->dx;
    meshj->dx=1.0;

    if(smokedrawtest2==1){
      meshj->norm[0]=1.0;
       meshj->norm[1]=0.0;
       meshj->norm[2]=0.0;
       meshj->smokedir=1;
       continue;
    }

    for(i=-9;i<=9;i++){
      if(i==0)continue;
      ii = i;
      if(i<0)ii=-i;
      norm[0]=0.0;
      norm[1]=0.0;
      norm[2]=0.0;
      switch (ii){
      case 1:
        if(i<0)norm[0]=-1.0;
        if(i>0)norm[0]=1.0;
        break;
      case 2:
        if(i<0)norm[1]=-1.0;
        if(i>0)norm[1]=1.0;
        break;
      case 3:
        if(i<0)norm[2]=-1.0;
        if(i>0)norm[2]=1.0;
        break;
      case 4:
        dx = meshj->xplt_orig[1]-meshj->xplt_orig[0];
        dy = meshj->yplt_orig[1]-meshj->yplt_orig[0];
        factor= dx*dx+dy*dy;
        if(factor==0.0){
          factor=1.0;
        }
        else{
          factor=1.0/sqrt(factor);
        }
        if(i<0){
          norm[0]=-dy*factor;
          norm[1]=-dx*factor;
        }
        else{
          norm[0]=dy*factor;
          norm[1]=dx*factor;
        }
        break;
      case 5:
        dx = meshj->xplt_orig[1]-meshj->xplt_orig[0];
        dy = meshj->yplt_orig[1]-meshj->yplt_orig[0];
        factor= dx*dx+dy*dy;
        if(factor==0.0){
          factor=1.0;
        }
        else{
          factor=1.0/sqrt(factor);
        }
        if(i<0){
          norm[0]= dy*factor;
          norm[1]=-dx*factor;
        }
        else{
          norm[0]=-dy*factor;
          norm[1]= dx*factor;
        }
        break;
      case 6:
        dy = meshj->yplt_orig[1]-meshj->yplt_orig[0];
        dz = meshj->zplt_orig[1]-meshj->zplt_orig[0];
        factor= dz*dz+dy*dy;
        if(factor==0.0){
          factor=1.0;
        }
        else{
          factor=1.0/sqrt(factor);
        }
        if(i<0){
          norm[1]=-dz*factor;
          norm[2]=-dy*factor;
        }
        else{
          norm[1]=dz*factor;
          norm[2]=dy*factor;
        }      
        break;
      case 7:
        dy = meshj->yplt_orig[1]-meshj->yplt_orig[0];
        dz = meshj->zplt_orig[1]-meshj->zplt_orig[0];
        factor= dz*dz+dy*dy;
        if(factor==0.0){
          factor=1.0;
        }
        else{
          factor=1.0/sqrt(factor);
        }
        if(i<0){
          norm[1]= dz*factor;
          norm[2]=-dy*factor;
        }
        else{
          norm[1]=-dz*factor;
          norm[2]= dy*factor;
        }
        break;
      case 8:
        dx = meshj->xplt_orig[1]-meshj->xplt_orig[0];
        dz = meshj->zplt_orig[1]-meshj->zplt_orig[0];
        factor= dz*dz+dx*dx;
        if(factor==0.0){
          factor=1.0;
        }
        else{
          factor=1.0/sqrt(factor);
        }
        if(i<0){
          norm[0]=-dz*factor;
          norm[2]=-dx*factor;
        }
        else{
          norm[0]=dz*factor;
          norm[2]=dx*factor;
        }      
        break;
      case 9:
        dx = meshj->xplt_orig[1]-meshj->xplt_orig[0];
        dz = meshj->zplt_orig[1]-meshj->zplt_orig[0];
        factor= dx*dx+dz*dz;
        if(factor==0.0){
          factor=1.0;
        }
        else{
          factor=1.0/sqrt(factor);
        }
        if(i<0){
          norm[0]= dz*factor;
          norm[2]=-dx*factor;
        }
        else{
          norm[0]=-dz*factor;
          norm[2]= dx*factor;
        }
        break;
      default:
        ASSERT(FFALSE);
        break;
      }
      scalednorm[0]=norm[0]*mscale[0];
      scalednorm[1]=norm[1]*mscale[1];
      scalednorm[2]=norm[2]*mscale[2];

      normdir[0] = mm[0]*scalednorm[0] + mm[4]*scalednorm[1] + mm[8]*scalednorm[2];
      normdir[1] = mm[1]*scalednorm[0] + mm[5]*scalednorm[1] + mm[9]*scalednorm[2];
      normdir[2] = mm[2]*scalednorm[0] + mm[6]*scalednorm[1] + mm[10]*scalednorm[2];

      cosangle = normdir[2]/sqrt(normdir[0]*normdir[0]+normdir[1]*normdir[1]+normdir[2]*normdir[2]);
      if(cosangle>1.0)cosangle=1.0;
      if(cosangle<-1.0)cosangle=-1.0;
      absangle=acos(cosangle)*180.0/pi;
      if(absangle<0.0)absangle=-absangle;
      if(absangle<minangle){
        iminangle=i;
        minangle=absangle;
        meshj->norm[0]=norm[0];
        meshj->norm[1]=norm[1];
        meshj->norm[2]=norm[2];
      }
    }
    meshj->smokedir=iminangle;
#ifdef pp_CULL
    if(meshj->smokedir!=meshj->smokedir_old){
      meshj->smokedir_old=meshj->smokedir;
      update_initcullplane=1;
#ifdef _DEBUG
      printf("mesh dir has changed\n");
#endif
    }
#endif
    if(demo_mode!=0){
      meshj->smokedir=1;
    }
  }
}
