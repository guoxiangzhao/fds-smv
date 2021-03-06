#define INTHREADER
#include "options.h"
#include <stdlib.h>
#include <stdio.h>

#include "smokeviewvars.h"
#include "IOvolsmoke.h"

/* ------------------ compress_svzip2 ------------------------ */

void compress_svzip2(void){
  char shellcommand[1024];

  PRINTF("Compressing...\n");
  compress_onoff(OFF);

  writeini(LOCAL_INI, NULL);

  // surround smokezip path name with "'s so that the system call can handle imbedded blanks

  strcpy(shellcommand, "\"");
  strcat(shellcommand, smokezippath);
  strcat(shellcommand, "\" ");
  if(overwrite_all == 1){
    strcat(shellcommand, " -f ");
  }
  if(erase_all == 1){
    strcat(shellcommand, " -c ");
  }
  if(compress_autoloaded == 1){
    strcat(shellcommand, " -auto ");
  }
  strcat(shellcommand, " ");
  strcat(shellcommand, smv_filename);

  PRINTF("Executing shell command: %s\n", shellcommand);
  system(shellcommand);
  updatesmoke3dmenulabels();
  updatepatchmenulabels();
  compress_onoff(ON);
  updatemenu = 1;
  PRINTF("Compression completed\n");
}

/* ------------------ init_all_threads ------------------------ */

void init_multi_threading(void){
#ifdef pp_THREAD
  pthread_mutex_init(&mutexCOMPRESS,NULL);
  pthread_mutex_init(&mutexVOLLOAD,NULL);
#endif
}

// *************** multi-threaded compression **************** 

#ifdef pp_THREAD
 /* ------------------ mt_compress_svzip ------------------------ */

void *mt_compress_svzip(void *arg){
  LOCK_COMPRESS
  compress_svzip2();
  updatemenu=1;
  UNLOCK_COMPRESS
  pthread_exit(NULL);
  return NULL;
}
#endif

/* ------------------ compress_svzip ------------------------ */
#ifdef pp_THREAD
void compress_svzip(void){
  pthread_create(&compress_thread_id,NULL,mt_compress_svzip,NULL);
}
#else
void compress_svzip(void){
  compress_svzip2();
}
#endif
// ************** multi threaded blockage smoothing **********************

/* ------------------ mt_update_smooth_blockages ------------------------ */
#ifdef pp_THREAD
void *mt_update_smooth_blockages(void *arg){

  if(ifsmoothblock()==1){
    PRINTF("Smoothing blockages in the background\n");
    update_smooth_blockages();
    updatefacelists=1;
  }
  pthread_exit(NULL);
  return NULL;
}
#endif

/* ------------------ mt_psystem ------------------------ */

#ifdef pp_THREAD
void *mt_psystem(void *arg){
  char command_line[1024], moviefile_path[1024];

  if(file_exists(get_moviefile_path(moviefile_path))==1){
    strcpy(command_line, "ffplay ");
    strcat(command_line, moviefile_path);
#ifdef WIN32
    strcat(command_line, " 2>Nul ");
#else
    strcat(command_line, " 2>/dev/null ");
#endif
    play_movie_now = 0;
    update_playmovie = 1;
    system(command_line);
    play_movie_now = 1;
    update_playmovie = 1;
  }
  pthread_exit(NULL);
  return NULL;
}
void psystem(char *commandline){
  pthread_create(&system_thread_id, NULL, mt_psystem, NULL);
}
#else
void psytem(char *commandline){
  system(commandline)
}
#endif

/* ------------------ smooth_blockages ------------------------ */

#ifdef pp_THREAD
void smooth_blockages(void){
  smoothing_blocks=1;
  pthread_create(&smooth_block_thread_id,NULL,mt_update_smooth_blockages,NULL);
}
#else
void smooth_blockages(void){
  smoothing_blocks=1;
    blocksneedsmoothing=ifsmoothblock();
    if(blocksneedsmoothing==1){
      update_smooth_blockages();
    }
}
#endif

/* ------------------ Update_Bounds ------------------------ */

int Update_Bounds(void){
  Update_All_Patch_Bounds();
#ifdef pp_THREAD
  pthread_join(update_all_patch_bounds_id,NULL);
#endif
  return 1;
}

/* ------------------ Update_All_Patch_Bounds_mt ------------------------ */

#ifdef pp_THREAD
void *Update_All_Patch_Bounds_mt(void *arg){
  Update_All_Patch_Bounds_st();
  pthread_exit(NULL);
  return NULL;
}
void Update_All_Patch_Bounds(void){
  pthread_create(&update_all_patch_bounds_id,NULL,Update_All_Patch_Bounds_mt,NULL);
}
#else
void Update_All_Patch_Bounds(void){
  Update_All_Patch_Bounds_st();
}
#endif

/* ------------------ mt_read_volsmoke_allframes_allmeshes2 ------------------------ */

#ifdef pp_THREAD
void mt_read_volsmoke_allframes_allmeshes2(void){
  pthread_create(&read_volsmoke_id,NULL,read_volsmoke_allframes_allmeshes2,NULL);
}
#endif
