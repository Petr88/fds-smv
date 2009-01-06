// $Date$ 
// $Revision$
// $Author$

#include "options.h"
#include <string.h>
#ifdef pp_OSX
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include "glui.h"
#include "flowfiles.h"
#define CPP
#include "ASSERT.h"
#include "smokeviewdefs.h"
#include "smokeviewvars.h"
#include "smokeheaders.h"

// svn revision character string
extern "C" char glui_stereo_revision[]="$Revision$";

GLUI *glui_stereo=NULL;
GLUI_Spinner *SPINNER_stereo_balance=NULL, *SPINNER_stereo_offset=NULL;
GLUI_Checkbox *CHECKBOX_stereo_frame=NULL;
GLUI_Checkbox *CHECKBOX_stereo_leftright=NULL;
GLUI_Checkbox *CHECKBOX_stereo_redblue=NULL;
GLUI_Checkbox *CHECKBOX_stereo_off=NULL;


#define STEREO_CLOSE 0
#define STEREO_RESET 2
#define STEREO_FRAME 3
#define STEREO_LEFTRIGHT 4
#define STEREO_OFF 5
#define STEREO_REDBLUE 6
#define SAVE_SETTINGS 999

void STEREO_CB(int var);

extern "C" void update_glui_stereo(void){
  int showstereoOLD;

  if(CHECKBOX_stereo_frame!=NULL){
    if(CHECKBOX_stereo_frame->get_int_val()!=stereo_frame)CHECKBOX_stereo_frame->set_int_val(stereo_frame);
  }
  if(CHECKBOX_stereo_leftright!=NULL){
    if(CHECKBOX_stereo_leftright->get_int_val()!=stereo_leftright)CHECKBOX_stereo_leftright->set_int_val(stereo_leftright);
  }
#ifdef pp_STEREO
  if(CHECKBOX_stereo_redblue!=NULL){
    if(CHECKBOX_stereo_redblue->get_int_val()!=stereo_redblue)CHECKBOX_stereo_redblue->set_int_val(stereo_redblue);
  }
#endif
  if(CHECKBOX_stereo_off!=NULL){
    if(CHECKBOX_stereo_off->get_int_val()!=stereo_off)CHECKBOX_stereo_off->set_int_val(stereo_off);
  }
  showstereoOLD=showstereo;
  if(stereo_frame==1)showstereo=1;
  if(stereo_leftright==1)showstereo=2;
#ifdef pp_STEREO
  if(stereo_redblue==1)showstereo=3;
#endif
  if(stereo_off==1)showstereo=0;
  if(showstereoOLD==3&&showstereo!=3){
    if(setbw!=setbwSAVE){
      setbw=1-setbwSAVE;
      ShadeMenu(2);
    }
  }
  else if(showstereoOLD!=3&&showstereo==3){
    setbwSAVE=setbw;
    if(setbw==0){
      setbwSAVE=setbw;
      ShadeMenu(2);
    }
  }
}
  


/* ------------------ glui_stereo_setup ------------------------ */

extern "C" void glui_stereo_setup(int main_window){
  if(glui_stereo!=NULL)glui_stereo->close();
  glui_stereo = GLUI_Master.create_glui("stereo",0,0,0);
  if(showgluistereo==0)glui_stereo->hide();
  
  SPINNER_stereo_balance=glui_stereo->add_spinner("Balance",GLUI_SPINNER_FLOAT,&pbalance);
  SPINNER_stereo_balance->set_float_limits(-5.0,5.0,GLUI_LIMIT_CLAMP);
  SPINNER_stereo_offset=glui_stereo->add_spinner("Offset",GLUI_SPINNER_FLOAT,&eoffset);
  SPINNER_stereo_offset->set_float_limits(-5.0,5.0,GLUI_LIMIT_CLAMP);

  CHECKBOX_stereo_off=glui_stereo->add_checkbox("Off",&stereo_off,STEREO_OFF,STEREO_CB);
  CHECKBOX_stereo_frame=glui_stereo->add_checkbox("Successive frames",&stereo_frame,STEREO_FRAME,STEREO_CB);
  if(videoSTEREO==0)CHECKBOX_stereo_frame->disable();
  CHECKBOX_stereo_leftright=glui_stereo->add_checkbox("Left/Right",&stereo_leftright,STEREO_LEFTRIGHT,STEREO_CB);
#ifdef pp_STEREO
  CHECKBOX_stereo_redblue=glui_stereo->add_checkbox("Red/Blue",&stereo_redblue,STEREO_REDBLUE,STEREO_CB);
#endif
  update_glui_stereo();

  glui_stereo->add_button("Reset",STEREO_RESET,STEREO_CB);
  glui_stereo->add_button("Save Settings",SAVE_SETTINGS,STEREO_CB);
  glui_stereo->add_button("Close",STEREO_CLOSE,STEREO_CB);
  
  glui_stereo->set_main_gfx_window( main_window );
}

/* ------------------ hide_glui_stereo ------------------------ */

extern "C" void hide_glui_stereo(void){
  if(glui_stereo!=NULL)glui_stereo->hide();
  showgluistereo=0;
  updatemenu=1;
}

/* ------------------ show_glui_stereo ------------------------ */

extern "C" void show_glui_stereo(void){
  if(glui_stereo!=NULL)glui_stereo->show();
}

/* ------------------ STEREO_CB ------------------------ */

void STEREO_CB(int var){

  switch (var){
  case STEREO_RESET:
    pbalance=pbalanceORIG;
    eoffset=eoffsetORIG;
    SPINNER_stereo_balance->set_float_val(pbalance);
    SPINNER_stereo_offset->set_float_val(eoffset);
    break;

  case STEREO_CLOSE:
    hide_glui_stereo();
    break;
  case STEREO_FRAME:
    stereo_leftright=0;
    stereo_off=0;
    if(stereo_frame==0)stereo_off=1;
    update_glui_stereo();
    updatemenu=1;
    break;
  case STEREO_LEFTRIGHT:
    stereo_frame=0;
    stereo_off=0;
#ifdef pp_STEREO
    stereo_redblue=0;
#endif
    if(stereo_leftright==0)stereo_off=1;
    update_glui_stereo();
    updatemenu=1;
    break;
#ifdef pp_STEREO
  case STEREO_REDBLUE:
    stereo_frame=0;
    stereo_off=0;
    if(stereo_redblue==1){
      stereo_off=0;
      stereo_leftright=0;
      stereo_frame=0;
    }
    else{
      stereo_off=1;
      stereo_leftright=0;
      stereo_frame=0;
    }
    update_glui_stereo();
    updatemenu=1;
    break;
#endif
  case STEREO_OFF:
#ifdef pp_STEREO
    stereo_redblue=0;
#endif
    if(stereo_off==1){
      stereo_frame=0;
      stereo_leftright=0;
    }
    else{
      if(videoSTEREO==1){
        stereo_frame=1;
        stereo_leftright=0;
      }
      else{
        stereo_leftright=1;
        stereo_frame=0;
      }
    }
    update_glui_stereo();
    updatemenu=1;
    break;
  case SAVE_SETTINGS:
    writeini(LOCAL_INI);
    break;
  }
}

