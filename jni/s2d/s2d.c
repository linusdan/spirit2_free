
  
  #define DEF_LOGTAG "sfl....."
  #define CLT_LOGTAG "sfc....."
  #define DMN_LOGTAG "sfd....."

  //#define LOGTAG "sfd....."
  //unsigned char logtag [16] = DEF_LOGTAG;//"s2l.....";   // Default "2s2l...." = JNI library

  unsigned char LOGTAG [16] = DEF_LOGTAG;//"s2l.....";   // Default "2s2l...." = JNI library

  const char * copyright = "Copyright (c) 2011-2015 Michael A. Reid. All rights reserved.";

  #include <dlfcn.h>
  #include <stdio.h>
  #include <errno.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  #include <sys/ioctl.h>

  #define DEF_BUF 512    // Raised from 256 so we can add headers to 255-256 byte buffers

  #define  GENERIC_CLIENT
  #define  GENERIC_SERVER

  #include "utils.c"

  #define  logv(...)  s2_log(ANDROID_LOG_VERBOSE,LOGTAG,__VA_ARGS__)
  #define  logd(...)  s2_log(ANDROID_LOG_DEBUG,LOGTAG,__VA_ARGS__)
  #define  logw(...)  s2_log(ANDROID_LOG_WARN,LOGTAG,__VA_ARGS__)
  #define  loge(...)  s2_log(ANDROID_LOG_ERROR,LOGTAG,__VA_ARGS__)

  //#ifdef  CS_AF_UNIX                                                      // For Address Family UNIX sockets
  //#include <sys/un.h>
  //#else                                                                   // For Address Family NETWORK sockets
  #include <netinet/in.h>
  #include <netdb.h> 

  int s2d_port = NET_PORT_S2D;


    // Spirit2 Daemon API support:

  #include "tnr/tnr_tnr.h"

    // Current info Variables:

    // Audio: 2 only
  char  curr_audio_mode         [16]= "Digital";
  char  curr_audio_state        [16]= "Stop";
//int curr_audio_mode_int = 0;
//int curr_audio_state_int = 0;
                                                                        // Simpler to use integer forms of variables
    // Chassis: 2 only
  #define PLUG_AUD_UNK_STR           "-1";
  char  curr_chass_plug_aud        [16]= "UNK";
int curr_chass_plug_aud_int = PLUG_AUD_UNK;                                // Should make this available to plugins !! tnr_bch needs and qcv antenna ??

  char  curr_chass_plug_tnr        [16]= "UNK";
int curr_chass_plug_tnr_int = PLUG_AUD_UNK;


    // Tuner Integer forms of variables:

int   curr_tuner_api_mode_int = 0;
int   curr_tuner_api_state_int = 0;
int   curr_tuner_mode_int = 0;
int   curr_tuner_state_int = 0;

int   curr_tuner_band_int     =  0;

  int   curr_tuner_freq_int         = -7;

int   curr_tuner_freq_lo_int     =  87500;
int   curr_tuner_freq_hi_int     = 108000;
//emph75
//rbds
    int   curr_tuner_freq_inc_int     = 100;                              // EU frequency increment = 100 KHz

  int   curr_tuner_vol_int         = -7;

int   curr_tuner_thresh_int        = -7;
int   curr_tuner_mute_int          = -7;
int   curr_tuner_stereo_int        = -7;
int   curr_tuner_seek_state_int    = -7;
int   curr_tuner_rds_state_int     = -7;
int   curr_tuner_rds_af_state_int  = -7;

  int   curr_tuner_rssi_int        = -7;
  int   curr_tuner_pilot_int       = 0;

int   curr_tuner_antenna_int       = 0;

  int   curr_tuner_rds_pi_int      = -7;
  int   curr_tuner_rds_pt_int      = -7;

    // Tuner String forms of variables:
  char  curr_tuner_api_mode     [16]= "UART";                           // 0 = UART / 1 = SHIM
  char  curr_tuner_api_state    [16]= "Stop";
  char  curr_tuner_mode         [16]= "Receive";
  char  curr_tuner_state        [16]= "Stop";

  char  curr_tuner_band         [16]= "EU";

  char  curr_tuner_freq         [16]= "0";//"107900";
  char  curr_tuner_vol          [16]= "65535";
  char  curr_tuner_thresh       [16]= "0";
char  curr_tuner_mute       [16]= "Mute";
  char  curr_tuner_stereo       [16]= "Mono";

  char  curr_tuner_seek_state   [16]= "Stop";                           // 0 = Stop, 1 = Up, 2 = Down
  char  curr_tuner_rds_state    [16]= "Stop";
  char  curr_tuner_rds_af_state [16]= "Stop";

  char  curr_tuner_rssi         [16]= "0";
  char  curr_tuner_pilot        [16]= "0";
  char  curr_tuner_extension    [16]= "0";

char  curr_tuner_antenna        [16]= "External";


  char  curr_tuner_rds_pi       [16]= "0";//"";
  char  curr_tuner_rds_pt       [16]= "0";//"-";
  char  curr_tuner_rds_ps       [16]= "";//-7";//"-";
  char  curr_tuner_rds_rt       [96]= "";//-7";//"-";


/*
  char * chass_plug_aud_get (int device) {
    switch (device) {
      case PLUG_AUD_UNK: return ("UNK");
      case PLUG_AUD_GEN: return ("GEN");
      case PLUG_AUD_GS1: return ("GS1");
      case PLUG_AUD_GS2: return ("GS2");
      case PLUG_AUD_GS3: return ("GS3");
      case PLUG_AUD_QCV: return ("QCV");
      case PLUG_AUD_OM7: return ("OM7");
      case PLUG_AUD_LG2: return ("LG2");
      case PLUG_AUD_XZ2: return ("XZ2");
    }
    return ("UNK");
  }
  char * chass_plug_tnr_get (int aud) {                                 // Get Tuner Plugin string from Audio Plugin integer
    switch (aud) {
      case PLUG_AUD_GS1: return ("SSL");
      case PLUG_AUD_GS2: return ("SSL");
      case PLUG_AUD_GS3: return ("SSL");
      case PLUG_AUD_QCV: return ("QCV");
      case PLUG_AUD_OM7: return ("BCH");
      case PLUG_AUD_LG2: return ("BCH");
      case PLUG_AUD_XZ2: return ("BCH");
      case PLUG_AUD_UNK: return ("UNK");
      case PLUG_AUD_GEN: return ("UNK");
    }
    return ("UNK");
  }
*/
/*

Set Audio Plugin:
    Set Audio Plugin string     from Audio Plugin string    =
    Set Audio Plugin integer    from Audio Plugin string    chass_plug_aud_int_get (char * aud)
    Set Tuner Plugin string     from Audio Plugin string    chass_plug_tnr_get (char * aud)
    Set Tuner Plugin integer    from Audio Plugin string    chass_plug_tnr_int_get (char * aud)

Set Tuner Plugin:       LATER !!
    Set Tuner Plugin string     from Tuner Plugin string    =
    Set Tuner Plugin integer    from Tuner Plugin string    chass_plug_tnr_int_get_tnr (char * tnr)

*/
  int chass_plug_aud_int_get (char * aud) {                             // Get Audio Plugin integer from Audio Plugin string
    if (! strcmp (aud, "UNK")) return (PLUG_AUD_UNK);
    if (! strcmp (aud, "GEN")) return (PLUG_AUD_UNK);
    if (! strcmp (aud, "GS1")) return (PLUG_AUD_GS1);
    if (! strcmp (aud, "GS2")) return (PLUG_AUD_GS2);
    if (! strcmp (aud, "GS3")) return (PLUG_AUD_GS3);
    if (! strcmp (aud, "QCV")) return (PLUG_AUD_QCV);
    if (! strcmp (aud, "OM7")) return (PLUG_AUD_OM7);
    if (! strcmp (aud, "LG2")) return (PLUG_AUD_LG2);
    if (! strcmp (aud, "XZ2")) return (PLUG_AUD_XZ2);
    return (PLUG_TNR_UNK);
  }

  int chass_plug_tnr_int_get (char * aud) {                             // Get Tuner Plugin integer from Audio Plugin string
    if (! strcmp (aud, "UNK")) return (PLUG_TNR_UNK);
    if (! strcmp (aud, "GEN")) return (PLUG_TNR_GEN);
    if (! strcmp (aud, "GS1")) return (PLUG_TNR_SSL);
    if (! strcmp (aud, "GS2")) return (PLUG_TNR_SSL);
    if (! strcmp (aud, "GS3")) return (PLUG_TNR_SSL);
    if (! strcmp (aud, "QCV")) return (PLUG_TNR_QCV);
    if (! strcmp (aud, "OM7")) return (PLUG_TNR_BCH);
    if (! strcmp (aud, "LG2")) return (PLUG_TNR_BCH);
    if (! strcmp (aud, "XZ2")) return (PLUG_TNR_BCH);
    return (PLUG_TNR_UNK);
  }

  char * chass_plug_tnr_get (char * aud) {                              // Get Tuner Plugin string from Audio Plugin string
    if (! strcmp (aud, "UNK")) return ("UNK");
    if (! strcmp (aud, "GEN")) return ("GEN");
    if (! strcmp (aud, "GS1")) return ("SSL");
    if (! strcmp (aud, "GS2")) return ("SSL");
    if (! strcmp (aud, "GS3")) return ("SSL");
    if (! strcmp (aud, "QCV")) return ("QCV");
    if (! strcmp (aud, "OM7")) return ("BCH");
    if (! strcmp (aud, "LG2")) return ("BCH");
    if (! strcmp (aud, "XZ2")) return ("BCH");
    return ("UNK");
  }

  int tuner_plugin_libname_get (char * tnr, char * lib_name, size_t max_size) {

    strlcpy (lib_name, "/data/data/fm.a2d.sf/lib/libs2t_", max_size);
    strlcat (lib_name, tnr, max_size);
    strlcat (lib_name, ".so", max_size);
    lower_set (lib_name);
    return (0);
/*
    switch (chass_plug_aud) {
      case PLUG_AUD_GEN: strlcpy (lib_name, "/data/data/fm.a2d.sf/lib/libs2t_gen.so", max_size);   return (chass_plug_aud);
      case PLUG_AUD_GS1: strlcpy (lib_name, "/data/data/fm.a2d.sf/lib/libs2t_ssl.so", max_size);   return (chass_plug_aud);
      case PLUG_AUD_GS2: strlcpy (lib_name, "/data/data/fm.a2d.sf/lib/libs2t_ssl.so", max_size);   return (chass_plug_aud);
      case PLUG_AUD_GS3: strlcpy (lib_name, "/data/data/fm.a2d.sf/lib/libs2t_ssl.so", max_size);   return (chass_plug_aud);
      case PLUG_AUD_QCV: strlcpy (lib_name, "/data/data/fm.a2d.sf/lib/libs2t_qcv.so", max_size);   return (chass_plug_aud);
      case PLUG_AUD_OM7: strlcpy (lib_name, "/data/data/fm.a2d.sf/lib/libs2t_bch.so", max_size);   return (chass_plug_aud);
      case PLUG_AUD_LG2: strlcpy (lib_name, "/data/data/fm.a2d.sf/lib/libs2t_bch.so", max_size);   return (chass_plug_aud);
      case PLUG_AUD_XZ2: strlcpy (lib_name, "/data/data/fm.a2d.sf/lib/libs2t_bch.so", max_size);   return (chass_plug_aud);
    }
    return (PLUG_AUD_UNK);
*/
  }


    // Plugin data:
  void           * tnr_fd       = NULL;
  plugin_funcs_t * tnr_funcs    = NULL;
  plugin_cbs_t   * tnr_cbs      = NULL;

    // Callback prototypes:
  void imp_cb_tuner_pilot (int new_pilot);
  void imp_cb_tuner_rds   (rds_struct_t * new_rds);
  void imp_cb_tuner_rssi  (int new_rssi);
  void imp_cb_tuner_rds_af(int new_freq);
  void imp_cb_tuner_state (int new_state);


    // First order of business is to determine the device we are running on, and set up appropriate plugins.
/*
  int chass_plug_tnr_set (int chass_plug_tnr) {
    return (curr_chass_plug_tnr_int);
  }
*/

  char * chass_plug_aud_set (char * aud) {

/*Set Audio Plugin:
    Set Audio Plugin string     from Audio Plugin string    =
    Set Audio Plugin integer    from Audio Plugin string    chass_plug_aud_int_get  (char * aud)
    Set Tuner Plugin string     from Audio Plugin string    chass_plug_tnr_get      (char * aud)
    Set Tuner Plugin integer    from Audio Plugin string    chass_plug_tnr_int_get  (char * aud)*/

    strcpy (curr_chass_plug_aud, aud);
    curr_chass_plug_aud_int = chass_plug_aud_int_get (aud);

    strcpy (curr_chass_plug_tnr, chass_plug_tnr_get (aud));
    curr_chass_plug_tnr_int = chass_plug_tnr_int_get (aud);

    if (curr_chass_plug_aud_int <= PLUG_AUD_UNK) {
      if (tnr_fd != NULL)                                               // Unload plugin and return resources
        dlclose (tnr_fd);
      tnr_fd = NULL;
      if (tnr_cbs != NULL)
        free (tnr_cbs);
      tnr_cbs = NULL;
      if (tnr_funcs != NULL)
        free (tnr_funcs);
      tnr_funcs = NULL;
    }

    tnr_funcs = calloc (1, sizeof (plugin_funcs_t));   // Setup functions
    if (tnr_funcs == NULL)
      return (curr_chass_plug_aud);

    tnr_cbs = calloc (1, sizeof (plugin_cbs_t));     // Setup callbacks
    if (tnr_cbs == NULL)
      return (curr_chass_plug_aud);

    char lib_name [DEF_BUF] = "";
    tuner_plugin_libname_get (curr_chass_plug_tnr, lib_name, sizeof (lib_name)); // Get Tuner Plugin library name

    tnr_fd = dlopen (lib_name, RTLD_LAZY);                              // Load library
    if (tnr_fd == NULL) {
      loge ("Could not load library '%s'", lib_name);
      return (curr_chass_plug_aud);
    }
    else {
      logd ("Loaded library %s  tnr_fd: %d", lib_name, tnr_fd);
    }

    tnr_cbs->cb_tuner_pilot   = imp_cb_tuner_pilot;                         // Setup callback functions structure
    tnr_cbs->cb_tuner_rds     = imp_cb_tuner_rds;
    tnr_cbs->cb_tuner_rssi    = imp_cb_tuner_rssi;
    tnr_cbs->cb_tuner_rds_af  = imp_cb_tuner_rds_af;
    tnr_cbs->cb_tuner_state   = imp_cb_tuner_state;

    plugin_reg_t plugin_reg = (plugin_reg_t) dlsym (tnr_fd, "plugin_reg"); // Dynamic link plugin_reg() function
    if (plugin_reg == NULL) {
      loge ("Could not find plugin_reg() symbol in loaded library '%s'", lib_name);
      dlclose (tnr_fd);
      return (curr_chass_plug_aud);
    }

    unsigned int ret_sig = 0;
    if (plugin_reg (& ret_sig, tnr_funcs, tnr_cbs) != 0) {                  // Register functions and get signature
      loge ("Loaded function plugin_reg() returned unsuccessful");
      dlclose (tnr_fd);
      return (curr_chass_plug_aud);
    }

    if (ret_sig != PLUGIN_SIG) {                                        // If signature mismatch
      loge ("Loaded function plugin_reg() returned successful but signature %x != ret_sig %x", PLUGIN_SIG, ret_sig);
      dlclose (tnr_fd);
      return (curr_chass_plug_aud);
    }

    //funcs_display ();                                                 // Show functions

    return (curr_chass_plug_aud);      
  }


    // Callback code:

    // Do nothing on callback; because we poll...
  void cb_tuner_change (char * key, char * val) {
    //logd ("cb_tuner_change key: %s  val: %s", key, val);
  }

    // Store the trimmed input string into the given output buffer, which must be large enough to store the result.  If it is too small, the output is truncated.
  size_t space_trim2 (char * out, size_t len, const char * str) {
    if(len == 0)
      return 0;

    const char * end;
    size_t out_size;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while( end > str && isspace (* end)) end --;
    end ++;

    // Set output size to minimum of trimmed string length and buffer size minus 1
    out_size = (end - str) < len-1 ? (end - str) : len-1;

    // Copy trimmed string and add null terminator
    memcpy (out, str, out_size);
    out [out_size] = 0;
    return (out_size);
  }

  char * space_trim (char * str) {
    char * end;
        // Trim trailing space
    end = str + strlen (str) - 1;
    while (end > str && isspace (* end))
      end --;
      // Write new null terminator
    * (end + 1) = 0;
    return str;
  }

  char * itop (int pilot) {
    if (pilot)
      return ("Stereo");
    else
      return ("Mono");
  }

  void imp_cb_tuner_pilot  (int new_pilot) {
    //logd ("imp_cb_tuner_pilot  new_pilot: %d", new_pilot);
    if (curr_tuner_pilot_int != new_pilot) {
      curr_tuner_pilot_int = new_pilot;
      strlcpy (curr_tuner_pilot, itop (new_pilot), sizeof (curr_tuner_pilot));
      cb_tuner_change ("tuner_pilot ", curr_tuner_pilot);
    }
  }

  void imp_cb_tuner_rds (rds_struct_t * new_rds) {
    //logd ("imp_cb_tuner_rds rds_struct srds_ps: \"%s\"  srds_rt: \"%s\"  ct: \"%d\"  ptyn: \"%d\"", new_rds->srds_ps, new_rds->srds_rt, new_rds->ct, new_rds->ptyn);
    //logd ("imp_cb_tuner_rds rds_struct: %p  freq: %d  pi: %d  tp: %d  srds_pt: %d  ta: %d  ms: %d  taf: %d  srds_af_num: %d", new_rds, freq, new_rds->srds_pi, new_rds->tp, new_rds->srds_pt, new_rds->ta, new_rds->ms, new_rds->taf, new_rds->srds_af_num);
    //for (ctr = 0; ctr < RDS_MAX_TMC; ctr ++)
    //  logd ("imp_cb_tuner_rds rds_struct tmc %d: %d", new_rds->tmc [ctr]);

    //logd ("imp_cb_tuner_rds freq: %d  rds_struct  pi: %d  srds_pt: %d srds_ps: \"%s\"  srds_rt: \"%s\"  srds_af_num: %d", freq, new_rds->srds_pi, new_rds->srds_pt, new_rds->srds_ps, new_rds->srds_rt, new_rds->srds_af_num);
    int ctr = 0;
    for (ctr = 0; ctr < new_rds->srds_af_num; ctr ++)
      logv ("imp_cb_tuner_rds rds_struct srds_af %d: %d", new_rds->srds_af [ctr]);

    char itoa_ret [MAX_ITOA_SIZE] = {0};
    if (curr_tuner_rds_pi_int != new_rds->srds_pi) {
      curr_tuner_rds_pi_int = new_rds->srds_pi;
      strlcpy (curr_tuner_rds_pi, itoa (curr_tuner_rds_pi_int, itoa_ret, 10), sizeof (curr_tuner_rds_pi));
      cb_tuner_change ("tuner_rds_pi", curr_tuner_rds_pi);
    }
    if (curr_tuner_rds_pt_int != new_rds->srds_pt) {
      curr_tuner_rds_pt_int = new_rds->srds_pt;
      strlcpy (curr_tuner_rds_pt, itoa (curr_tuner_rds_pt_int, itoa_ret, 10), sizeof (curr_tuner_rds_pt));
      cb_tuner_change ("tuner_rds_pt", curr_tuner_rds_pt);
    }
    if (strcmp (curr_tuner_rds_ps, new_rds->srds_ps)) {
      strlcpy (curr_tuner_rds_ps, new_rds->srds_ps, sizeof (curr_tuner_rds_ps));
      cb_tuner_change ("tuner_rds_ps", new_rds->srds_ps);
    }

    //logd ("Radiotext: \"%s\"", new_rds->srds_rt);
    //    space_trim (new_rds->srds_rt);
    //logd ("Radiotext: \"%s\"", new_rds->srds_rt);
    if (strcmp (curr_tuner_rds_rt, new_rds->srds_rt)) {
      strlcpy (curr_tuner_rds_rt, new_rds->srds_rt, sizeof (curr_tuner_rds_rt));
      cb_tuner_change ("tuner_rds_rt", new_rds->srds_rt);
    }
  }

  void imp_cb_tuner_rssi (int new_rssi) {
    //logd ("imp_cb_tuner_rssi new_rssi: %d", new_rssi);        // rssi: (760 - 347) / 19 = 21.74. Thus internal range thus is 0 - 46
    char itoa_ret [MAX_ITOA_SIZE] = {0};
    if (curr_tuner_rssi_int != new_rssi) {
      curr_tuner_rssi_int = new_rssi;
      strlcpy (curr_tuner_rssi, itoa (curr_tuner_rssi_int, itoa_ret, 10), sizeof (curr_tuner_rssi));
      cb_tuner_change ("tuner_rssi", curr_tuner_rssi);
    }
  }

  void imp_cb_tuner_rds_af (int new_freq) {
    logd ("imp_cb_tuner_rds_af new_freq: %d", new_freq);
  }

  void imp_cb_tuner_state (int new_state) {
    logd ("imp_cb_tuner_state new_state: %d", new_state);
    strlcpy (curr_tuner_state, "Stop", sizeof (curr_tuner_state));
    cb_tuner_change ("tuner_state", curr_tuner_state);
  }


    // Audio:

  #include "alsa.c"

  #include "aud_all.c"                                                  // Audio functions for all supported audio architectures. To be replaced with audio plugins, similar to tuner plugins.


    // API: key_get(key)        Get value of key    eg: get(tuner_rssi)
    //      key_set(key,val)    Set value           eg: set(tuner_state,start)

    // To process get requests:
  int key_get (char * key, char * res_buf, int res_max, char * val) {
    char itoa_ret [MAX_ITOA_SIZE] = {0};                                // Temp buffer for itoa()

    strlcpy (res_buf, "Key Unknown", res_max);                          // Default = "g key -9999"

    if (0)      loge ("key_get never 0");

            // Values we can just read from memory:

    else if (! strcmp (key, "chass_plug_aud"))
      strlcpy (res_buf, curr_chass_plug_aud,        res_max);
    else if (! strcmp (key, "chass_plug_tnr"))
      strlcpy (res_buf, curr_chass_plug_tnr,        res_max);

    else if (! strcmp (key, "audio_state"))
      strlcpy (res_buf, curr_audio_state,           res_max);
    else if (! strcmp (key, "audio_mode"))
      strlcpy (res_buf, curr_audio_mode,            res_max);

    else if (! strcmp (key, "tuner_api_state"))
      strlcpy (res_buf, curr_tuner_api_state,       res_max);
    else if (! strcmp (key, "tuner_api_mode"))
      strlcpy (res_buf, curr_tuner_api_mode,        res_max);
    else if (! strcmp (key, "tuner_mode"))
      strlcpy (res_buf, curr_tuner_mode,            res_max);
    else if (! strcmp (key, "tuner_state"))
      strlcpy (res_buf, curr_tuner_state,           res_max);

    else if (! strcmp (key, "tuner_band"))
      strlcpy (res_buf, curr_tuner_band,            res_max);
    else if (! strcmp (key, "tuner_seek_state"))
      strlcpy (res_buf, curr_tuner_seek_state,      res_max);
    else if (! strcmp (key, "tuner_rds_state"))
      strlcpy (res_buf, curr_tuner_rds_state,       res_max);
    else if (! strcmp (key, "tuner_rds_af_state"))
      strlcpy (res_buf, curr_tuner_rds_af_state,    res_max);
    else if (! strcmp (key, "tuner_extension"))
      strlcpy (res_buf, curr_tuner_extension,       res_max);
    else if (! strcmp (key, "tuner_stereo"))
      strlcpy (res_buf, curr_tuner_stereo,          res_max);
    else if (! strcmp (key, "tuner_rds_pi"))
      strlcpy (res_buf, curr_tuner_rds_pi,          res_max);
    else if (! strcmp (key, "tuner_rds_pt"))
      strlcpy (res_buf, curr_tuner_rds_pt,          res_max);
    else if (! strcmp (key, "tuner_rds_ps"))
      strlcpy (res_buf, curr_tuner_rds_ps,          res_max);
    else if (! strcmp (key, "tuner_rds_rt"))
      strlcpy (res_buf, curr_tuner_rds_rt,          res_max);


  #define DEVICE_KNOWN (curr_chass_plug_aud_int > PLUG_AUD_UNK)

            // Values we get from the FM chip:

    else if (! strcmp (key, "tuner_thresh")) {
      if (DEVICE_KNOWN)
        strlcpy (curr_tuner_thresh, itoa (curr_tuner_thresh_int = tnr_funcs->tnr_tuner_thresh_sg(GET), itoa_ret, 10),  sizeof (curr_tuner_thresh));
      strlcpy (res_buf, curr_tuner_thresh,          res_max);
    }
    else if (! strcmp (key, "tuner_freq")) {
      if (DEVICE_KNOWN)
        strlcpy (curr_tuner_freq,   itoa (curr_tuner_freq_int   = tnr_funcs->tnr_tuner_freq_sg  (GET), itoa_ret, 10),  sizeof (curr_tuner_freq));
      strlcpy (res_buf, curr_tuner_freq,            res_max);
    }
    else if (! strcmp (key, "tuner_vol")) {
      //if (DEVICE_KNOWN)
      //  strlcpy (curr_tuner_vol,    itoa (curr_tuner_vol_int    = tnr_funcs->tnr_tuner_vol_sg   (GET), itoa_ret, 10),  sizeof (curr_tuner_vol));
      strlcpy (res_buf, curr_tuner_vol,             res_max);
    }
    else if (! strcmp (key, "tuner_rssi")) {
      if (DEVICE_KNOWN)
        strlcpy (curr_tuner_rssi,   itoa (curr_tuner_rssi_int   = tnr_funcs->tnr_tuner_rssi_sg  (GET), itoa_ret, 10),  sizeof (curr_tuner_rssi));
      strlcpy (res_buf, curr_tuner_rssi,            res_max);
    }
    else if (! strcmp (key, "tuner_pilot ")) {
      if (DEVICE_KNOWN)
        strlcpy (curr_tuner_pilot,  itop (curr_tuner_pilot_int  = tnr_funcs->tnr_tuner_pilot_sg (GET)),                sizeof (curr_tuner_pilot));
      strlcpy (res_buf, curr_tuner_pilot,            res_max);
    }

    else if (! strcmp (key, "tuner_bulk")) {                        // Tuner Bulk:  Update freq, rssi, pilot from tuner plugin

      if (DEVICE_KNOWN)
        strlcpy (curr_tuner_freq,   itoa (curr_tuner_freq_int   = tnr_funcs->tnr_tuner_freq_sg  (GET), itoa_ret, 10),  sizeof (curr_tuner_freq));

      if (DEVICE_KNOWN)
        strlcpy (curr_tuner_rssi,   itoa (curr_tuner_rssi_int   = tnr_funcs->tnr_tuner_rssi_sg  (GET), itoa_ret, 10),  sizeof (curr_tuner_rssi));

//!!!! May interfere with RDS on SSL, so disable for now !!  (used to interfere with BCH also)
      //if (DEVICE_KNOWN)
      //  strlcpy (curr_tuner_pilot,  itop (curr_tuner_pilot_int  = tnr_funcs->tnr_tuner_pilot_sg (GET)),                sizeof (curr_tuner_pilot));

      res_buf [0] = 0;
      strlcat (res_buf, curr_tuner_freq,            res_max);

      strlcat (res_buf, "mArK",                     res_max);
      strlcat (res_buf, curr_tuner_rssi,            res_max);

      strlcat (res_buf, "mArK",                     res_max);
      strlcat (res_buf, curr_tuner_pilot,            res_max);
                                                                        // Update PI, PT, PS, RT from memory
      strlcat (res_buf, "mArK",                     res_max);
      strlcat (res_buf, curr_tuner_rds_pi,          res_max);

      strlcat (res_buf, "mArK",                     res_max);
      strlcat (res_buf, curr_tuner_rds_pt,          res_max);

      strlcat (res_buf, "mArK",                     res_max);
      strlcat (res_buf, curr_tuner_rds_ps,          res_max);

      strlcat (res_buf, "mArK",                     res_max);
      strlcat (res_buf, curr_tuner_rds_rt,          res_max);
    }

    return (strlen (res_buf));                                          // !!!! FIX For International RDS PS and RT
  }

#ifdef  SEE_AUD_ALL_C
/* Audio calibration related functions */
#endif  // #ifdef  SEE_AUD_ALL_C

  char * param2_get (char * params) {
    int ctr = 0;
    int len = strlen (params);
    for (ctr = 0; ctr < len - 1; ctr ++) {
      if (params [ctr] == ' ') {
        params [ctr] = 0;                                               // ASCIIZ terminate for parameter 1 !!
        return (& params [ctr + 1]);
      }
    }
    return (NULL);
  }

    // To process set requests:
  int key_set (char * key, char * res_buf, int res_max, char * val) {
    int     ret = 0;
    char * pret = NULL;
    int    sval = -99999943;
    //char itoa_ret [MAX_ITOA_SIZE] = {0};
    char * p2 = NULL;

    if (0)      loge ("key_set never 0");

    else if (! strcmp (key, "audio_alsa_log")) {
      loge ("Got audio_alsa_log");
      itoa (alsa_control_log (atoi (val)), res_buf, 10);
    }

    else if (! strcmp (key, "audio_alsa_bool")) {
      p2 = param2_get (val);                                            // val now ASCIIZ terminated for param1
      if (p2 == NULL)
        itoa (alsa_bool_get (val,        -1), res_buf, 10);
      else
        itoa (alsa_bool_set (val, atoi (p2)), res_buf, 10);
    }
    else if (! strcmp (key, "audio_alsa_enum")) {
      p2 = param2_get (val);                                            // val now ASCIIZ terminated for param1
      if (p2 == NULL)
        itoa (alsa_enum_get (val,        -1), res_buf, 10);
      else
        itoa (alsa_enum_set (val, atoi (p2)), res_buf, 10);
    }
    else if (! strcmp (key, "audio_alsa_long")) {
      p2 = param2_get (val);                                            // val now ASCIIZ terminated for param1
      if (p2 == NULL)
        itoa (alsa_long_get (val,        -1), res_buf, 10);
      else
        itoa (alsa_long_set (val, atoi (p2)), res_buf, 10);
    }

    else if (! strcmp (key, "tuner_acdb")) {                        // If Tuner ACDB...
      //loge ("GOT tuner_acdb: %d", acdb_disable ());
    }

    else if (! strcmp (key, "chass_plug_aud")) {                    // If chass_plug_aud...
      logd ("key_set chass_plug_aud_set: %s", pret = chass_plug_aud_set (val));
    }
/*
    else if (! strcmp (key, "chass_plug_tnr")) {                    // If chass_plug_tnr...
      logd ("key_set chass_plug_tnr_set: %d", ret = curr_chass_plug_tnr_int = chass_plug_tnr_set (sval = atoi (val)));
      if (ret == sval)
        strlcpy (curr_chass_plug_tnr, val,  sizeof (curr_chass_plug_tnr));
    }
*/
    else if (! strcmp (key, "tuner_mode")) {                        // If Tuner Mode...
      if (! strcmp (val, "Transmit"))
        logd ("key_set tuner_mode transmit: %d", ret = curr_tuner_mode_int = tnr_funcs->tnr_tuner_mode_sg (sval = 1));
      else if (! strcmp (val, "Receive"))
        logd ("key_set tuner_mode receive:  %d", ret = curr_tuner_mode_int = tnr_funcs->tnr_tuner_mode_sg (sval = 0));
      if (ret == sval)
        strlcpy (curr_tuner_mode, val,  sizeof (curr_tuner_mode));
    }

    else if (! strcmp (key, "tuner_api_mode")) {                    // If Tuner API Mode...
      if (! strcmp (val, "UART"))
        logd ("key_set tuner_api_mode UART: %d", ret = curr_tuner_api_mode_int = tnr_funcs->tnr_tuner_api_mode_sg (sval = 0));
      else if (! strcmp (val, "SHIM"))
        logd ("key_set tuner_api_mode SHIM: %d", ret = curr_tuner_api_mode_int = tnr_funcs->tnr_tuner_api_mode_sg (sval = 1));
      if (ret == sval)
        strlcpy (curr_tuner_api_mode, val,  sizeof (curr_tuner_api_mode));
    }

    else if (! strcmp (key, "tuner_api_state")) {                   // If Tuner API State...
      if (! strcmp (val, "Start"))
        logd ("key_set tuner_api_state start: %d", ret = curr_tuner_api_state_int = tnr_funcs->tnr_tuner_api_state_sg (sval = 1));
      else if (! strcmp (val, "Stop")) {
        logd ("key_set tuner_api_state stop:  %d", ret = curr_tuner_api_state_int = tnr_funcs->tnr_tuner_api_state_sg (sval = 0));
        gen_server_exiting = 1;
      }
      if (ret == sval)
        strlcpy (curr_tuner_api_state, val,  sizeof (curr_tuner_api_state));
    }


    // Remaining set commands require valid curr_chass_plug_aud_int:

    else if (! DEVICE_KNOWN) {
      loge ("key_set ! DEVICE_KNOWN");
    }

        // Audio:
    else if (! strcmp (key, "audio_state")) {                       // Audio State
      logd ("key_set audio_state: %s", audio_state_set (val));
    }
    else if (! strcmp (key, "audio_mode")) {                        // Audio Mode
      logd ("key_set audio_mode: %s", audio_mode_set (val));
    }

        // Tuner:

    else if (! strcmp (key, "tuner_state")) {                       // Tuner State
      if (! strcmp (val, "Start")) {                                // If Start...
        logd ("key_set tuner_state_sg Start: %d", ret = curr_tuner_state_int = tnr_funcs->tnr_tuner_state_sg (sval = 1));
      }
      else if (! strcmp (val, "Stop")) {                            // If Stop...
        //gen_server_exiting = 1;
        logd ("key_set tuner_state_sg Stop: %d",  ret = curr_tuner_state_int = tnr_funcs->tnr_tuner_state_sg (sval = 0));
      }
      if (ret == sval)
        strlcpy (curr_tuner_state, val,  sizeof (curr_tuner_state));
    }

    else if (! strcmp (key, "tuner_band")) {                        // If Tuner Band...
      if (! strcmp (val, "EU"))                                     // If EU...
        logd ("key_set tuner_band_sg Start: %d", ret = curr_tuner_band_int = tnr_funcs->tnr_tuner_band_sg (sval = 0));
      else if (! strcmp (val, "US"))                                // If US...
        logd ("key_set tuner_band_sg Stop: %d",  ret = curr_tuner_band_int = tnr_funcs->tnr_tuner_band_sg (sval = 1));
      if (ret == sval) {
        strlcpy (curr_tuner_band, val,  sizeof (curr_tuner_band));
        curr_tuner_freq_inc_int = 100;
        if (sval == 1)
          curr_tuner_freq_inc_int = 200;
      }
    }

    else if (! strcmp (val, "tuner_mute")) {                        // If Tuner Mute...
      if (! strcmp (val, "Mute"))                                   // If Mute...
        logd ("key_set tuner_mute_sg Start: %d", ret = curr_tuner_mute_int = tnr_funcs->tnr_tuner_mute_sg (sval = 1));
      else if (! strcmp (val, "Unmute"))                            // If Unmute...
        logd ("key_set tuner_mute_sg Stop: %d",  ret = curr_tuner_mute_int = tnr_funcs->tnr_tuner_mute_sg (sval = 0));
      if (ret == sval)
        strlcpy (curr_tuner_mute, val,  sizeof (curr_tuner_mute));
    }

    else if (! strcmp (key, "tuner_seek_state")) {                  // If Tuner Seek State...
      if (! strcmp (val, "Up"))
        logd ("key_set tuner_seek_state up:   %d", ret = curr_tuner_seek_state_int = tnr_funcs->tnr_tuner_seek_state_sg (sval = 1));
      else if (! strcmp (val, "Down"))
        logd ("key_set tuner_seek_state down: %d", ret = curr_tuner_seek_state_int = tnr_funcs->tnr_tuner_seek_state_sg (sval = 2));
      else if (! strcmp (val, "Stop"))
        logd ("key_set tuner_seek_state stop: %d", ret = curr_tuner_seek_state_int = tnr_funcs->tnr_tuner_seek_state_sg (sval = 0));
      if (ret == sval)
        strlcpy (curr_tuner_seek_state, val,  sizeof (curr_tuner_seek_state));
    }
    else if (! strcmp (key, "tuner_rds_state")) {                   // If Tuner RDS State...
      if (! strcmp (val, "Start"))                                  // Start
        logd ("key_set tuner_rds_state_sg: %d", ret = curr_tuner_rds_state_int = tnr_funcs->tnr_tuner_rds_state_sg (sval = 1));
      else if (! strcmp (val, "Stop"))                              // Stop
        logd ("key_set tuner_rds_state_sg: %d", ret = curr_tuner_rds_state_int = tnr_funcs->tnr_tuner_rds_state_sg (sval = 0));
      if (ret == sval)
        strlcpy (curr_tuner_rds_state, val,  sizeof (curr_tuner_rds_state));
    }
    else if (! strcmp (key, "tuner_rds_af_state")) {                // Tuner RDS AF State
      if (! strcmp (val, "Start"))                                  // Start
        logd ("key_set tuner_rds_af_state_sg: %d", ret = curr_tuner_rds_af_state_int = tnr_funcs->tnr_tuner_rds_af_state_sg (sval = 1));
      else if (! strcmp (val, "Stop"))                              // Stop
        logd ("key_set tuner_rds_af_state_sg: %d", ret = curr_tuner_rds_af_state_int = tnr_funcs->tnr_tuner_rds_af_state_sg (sval = 0));
      if (ret == sval)
        strlcpy (curr_tuner_rds_af_state, val,  sizeof (curr_tuner_rds_af_state));
    }
    else if (! strcmp (key, "tuner_stereo")) {                      // Tuner Stereo
      if (! strcmp (val, "Stereo"))                                 // Stereo
        logd ("key_set tuner_stereo_sg: %d", ret = curr_tuner_stereo_int = tnr_funcs->tnr_tuner_stereo_sg (sval = 1));
      else if (! strcmp (val, "Mono"))                              // Mono
        logd ("key_set tuner_stereo_sg: %d", ret = curr_tuner_stereo_int = tnr_funcs->tnr_tuner_stereo_sg (sval = 0));
      if (ret == sval)
        strlcpy (curr_tuner_stereo, val,  sizeof (curr_tuner_stereo));
    }

    else if (! strcmp (key, "tuner_freq")) {                        // Tuner Freq
      logd ("key_set tuner_freq_sg: %d", ret = curr_tuner_freq_int = tnr_funcs->tnr_tuner_freq_sg (sval = atoi (val)));
      if (ret == sval)
        strlcpy (curr_tuner_freq, val,  sizeof (curr_tuner_freq));
    }
    else if (! strcmp (key, "tuner_vol")) {                         // Tuner Vol
      /*if (curr_chass_plug_aud_int == PLUG_AUD_GS1)                           // GS1 volume controlled by chip
        logd ("key_set tuner_vol -> audio_ana_vol: %d", ret = curr_tuner_vol_int = tnr_funcs->tnr_tuner_vol_sg (sval = atoi (val)));
      else*/
        logd ("key_set tuner_vol -> audio_ana_vol: %d", ret = curr_tuner_vol_int =           audio_ana_vol_set (sval = atoi (val)));
      if (ret == sval)
        strlcpy (curr_tuner_vol, val,  sizeof (curr_tuner_vol));
    }
    else if (! strcmp (key, "tuner_thresh")) {                      // Tuner Thresh
      logd ("key_set tuner_thresh_sg: %d", ret = curr_tuner_thresh_int = tnr_funcs->tnr_tuner_thresh_sg (sval = atoi (val)));
      if (ret == sval)
        strlcpy (curr_tuner_thresh, val,  sizeof (curr_tuner_thresh));
    }
    else if (! strcmp (key, "tuner_rds_pi")) {                      // Tuner RDS PI
      strlcpy (curr_tuner_rds_pi, val,  sizeof (curr_tuner_rds_pi));
      logd ("key_set tuner_rds_pi_sg: %d", ret = curr_tuner_rds_pi_int = tnr_funcs->tnr_tuner_rds_pi_sg (sval = atoi (val)));
      if (ret == sval)
        strlcpy (curr_tuner_rds_pi, val,  sizeof (curr_tuner_rds_pi));
    }
    else if (! strcmp (key, "tuner_rds_pt")) {                      // Tuner RDS PT
      strlcpy (curr_tuner_rds_pt, val,  sizeof (curr_tuner_rds_pt));
      logd ("key_set tuner_rds_pt_sg: %d", ret = curr_tuner_rds_pt_int = tnr_funcs->tnr_tuner_rds_pt_sg (sval = atoi (val)));
      if (ret == sval)
        strlcpy (curr_tuner_rds_pt, val,  sizeof (curr_tuner_rds_pt));
    }

    else if (! strcmp (key, "tuner_rds_ps")) {                      // Tuner RDS PS
      logd ("key_set tuner_rds_ps_set: %s", pret = tnr_funcs->tnr_tuner_rds_ps_sg (val));
      if (pret)
        strlcpy (curr_tuner_rds_ps, pret,  sizeof (curr_tuner_rds_ps));
    }
    else if (! strcmp (key, "tuner_rds_rt")) {                      // Tuner RDS RT
      logd ("key_set tuner_rds_rt_set: %s", pret = tnr_funcs->tnr_tuner_rds_rt_sg (val));
      if (pret)
        strlcpy (curr_tuner_rds_rt, pret,  sizeof (curr_tuner_rds_rt));
    }

    else if (! strcmp (key, "tuner_extension")) {                   // Tuner Extension
      logd ("key_set tuner_extension_sg: %d", pret = tnr_funcs->tnr_tuner_extension_sg (val));
      if (pret)
        strlcpy (curr_tuner_extension, pret,  sizeof (curr_tuner_extension));
    }

    if (pret)                                                           // If pointer return not NULL...
      return (0);//-321);
    else    
      return (ret);
  }

    // Generic server function that deals with commands and returns a response:
  int gen_server_loop_func (unsigned char * cmd_buf, int cmd_len, unsigned char * res_buf, int res_max) {

    if (res_buf == NULL) {
      loge ("gen_server_loop_func res_buf == NULL: %p", res_buf);
      return (-99);
    }

    strlcpy (res_buf, "-543", res_max);                                 // Default for unknown command and other fatal errors

    if (ena_log_s2d_cmd && cmd_buf != NULL)
      logd ("gen_server_loop_func start res_max: %d  cmd_len: %d  cmd_buf: \"%s\"", res_max, cmd_len, cmd_buf);
    else if (cmd_buf == NULL) {
      loge ("gen_server_loop_func start res_max: %d  cmd_len: %d  cmd_buf: %p", res_max, cmd_len, cmd_buf);
      strlcpy (res_buf, "cmd_buf == NULL", res_max);
      return (strlen (res_buf));
    }

    if (strlen (cmd_buf) < 1) {                                         // Error for short line
      strlcpy (res_buf, "Short command", res_max);
      loge ("gen_server_loop_func: %s", res_buf);
      return (strlen (res_buf));
    }

    int ret = 0;
    char * key = & cmd_buf [0];                                         // Pointer to key   passed to us: starts at 2nd word, 3rd character
    char * val = & cmd_buf [1];                                         // Pointer to value passed to us: add size of key (sky) later when determined

    int len_left = strlen (val);
    int is_set = 0;
    int ctr = 0;
    for (ctr = 0; ctr < len_left - 1; ctr ++) {                         // Only look for space to 2nd last character, to leave room for a 1 character value
      if (val [ctr] == ' ') {
        val [ctr] = 0;                                                  // Null terminate for key
        is_set = 1;                                                     // Have a value, so this is a set
        val = & val [ctr + 1];                                          // Point after space
        break;
      }
    }
    if (is_set) {                                                       // If a SET (and get) operation
      if (ena_log_s2d_cmd)
        logd ("gen_server_loop_func prior to key_set key: %s  val: %s", key, val);

      ret = key_set (key, res_buf, res_max, val);                       // SET value

      if (ena_log_s2d_cmd)
        logd ("gen_server_loop_func after    key_set key: %s  val: %s  ret: %d", key, val, ret);
    }
    else {                                                              // Else if just a GET operation
      if (ena_log_s2d_cmd)
        logd ("gen_server_loop_func prior to get key: %s", key);
    }

    int res_len = key_get (key, res_buf, res_max, val);                 // GET value: (And do SET Response)

    if (res_len >= 0) {
      int asciiz_idx = res_len + 1;
      if (asciiz_idx > res_max - 1) {
        asciiz_idx = res_max - 1;
        loge ("asciiz_idx > res_max - 1");
      }
      res_buf [asciiz_idx] = 0;
    }

    if (ena_log_s2d_cmd)
      logd ("gen_server_loop_func after    key_get key: %s  res_len: %d  res_buf: \"%s\"", key, res_len, res_buf);

    return (res_len);                                                   // Return size of result
  }

    // Generic Server code calls every 100+ ms or so. Polling is MUCH more reliable than threading in Android Bionic C-look alike...
    // Equivalent tuner plugin function is called for plugin to do it's pollawable work
  int gen_server_poll_func (int poll_ms) {
    int ret = 0;

    if (ena_log_verbose_tshoot)
      logd ("gen_server_poll_func before tnr_funcs->tnr_tuner_event_sg poll_ms: %d  tnr_funcs: %p", poll_ms, tnr_funcs);

    if (tnr_funcs != NULL)
      ret = tnr_funcs->tnr_tuner_event_sg (NULL);//poll_ms);

    if (ena_log_verbose_tshoot)
      logd ("gen_server_poll_func after  tnr_funcs->tnr_tuner_event_sg poll_ms: %d  tnr_funcs: %p", poll_ms, tnr_funcs);

    return (ret);
  }


    // Main client and server code:

  char * binary_description = /*"Mike's really cool*/ "Spirit2 FM Receiver/Transmitter daemon version: ";
  char * s2d_running_file = "/dev/s2d_running";

  int client_run () {
    logd ("main client mode");
    char store_line [DEF_BUF] = {0};
    char * line = store_line;
    while (line != NULL) {
      //line = readline (store_line);                                   // Caller must free malloc'd memory
      line = fgets (store_line, sizeof (store_line), stdin);          // Never  use  gets()
      //logd ("line: %p", line);

      if (line != NULL) {
        int cmd_len = strlen (line);// + 1;
        if (cmd_len > 0 && cmd_len < sizeof (store_line)) {
          char * cmd_buf = line;
          if (cmd_buf [cmd_len - 1] == '\r' || cmd_buf [cmd_len - 1] == '\n')
            cmd_buf [cmd_len - 1] = 0;

          logd ("main To    gen_client_cmd cmd_len: %d  cmd_buf: \"%s\"", cmd_len, cmd_buf);
          unsigned char res_buf [RES_DATA_MAX] = {0};

          int res_len = gen_client_cmd (cmd_buf, cmd_len + 1, res_buf, sizeof (res_buf), s2d_port, 1000);  // Send command at line / store_line and get response to res_buf
          logd ("main res_len: %d  res_buf: \"%s\"", res_len, res_buf);
        }
      }
    }
    logd ("main done client: %s %s", binary_description, manifest_version);
    return (0);                                                         // Done client mode
  }

  int server_run () {
    killall ("libs2d.so", 1);                                           // Kill all other running instances of this libs2d.so daemon, except this one

    int nochdir = 0;                                                    // 0 = Change current working directory to root "/"
    int noclose = 0;                                                    // 0 = Redirect standard input, standard output and standard error to /dev/null
    int ret = daemon (nochdir, noclose);                                // Daemonize: Detach from controlling terminal and run in the background as system daemon
    logd ("main daemon() ret: %d", ret);

    gen_server_exiting = 0;
    while (! gen_server_exiting) {                                      // Do server work until gen_server_exiting
      file_write (s2d_running_file, "", 0, O_CREAT | O_RDWR);                   // Signal running

      gen_server_loop (s2d_port, S2D_POLL_MS);                          // Continue servicing requests on s2d_port with 100 ms polling

      if (! gen_server_exiting) {                                       // Must be an error  if gen_server_loop() returned & not gen_server_exiting
        loge ("main gen_server_loop() returned & not gen_server_exiting");
        ms_sleep (3000);                                                // Take a rest: Sleep it off-*////////////////////////////
      }
      else {
        logd ("main gen_server_loop() returned & gen_server_exiting");
        file_delete (s2d_running_file);                                         // Signal NOT running
      }
    }
      // When done server mode:
    //curr_tuner_state_int = tnr_funcs->tnr_state_sg (0);               // Ensure FM is turned off, but this can hang... do disable and see Feb 9, 2015
    //strlcpy (curr_tuner_state, "Stop", sizeof (curr_tuner_state));
    //logd ("main tuner_state_sg: %d", ret);
    //chass_plug_aud_set (PLUG_AUD_UNK);                                     // Process exit takes care of unloading libraries. Don't chance the hang...
    //curr_chass_plug_aud_int = PLUG_AUD_UNK;                                // Not initialized

    logd ("main done server: %s %s", binary_description, manifest_version);
    return (0);                                                         // Done client mode
  }

    // Main entrance:

  int main (int argc, char ** argv) {
    int server_mode = 0;
    if (argc > 1)                                                       // If server mode... (If 1 or more parameters)
      server_mode = 1;

    if (server_mode)                                                    // If server mode...
      strlcpy (LOGTAG, DMN_LOGTAG, sizeof (LOGTAG));                    // Server daemon logtag
    else
      strlcpy (LOGTAG, CLT_LOGTAG, sizeof (LOGTAG));                    // Client/tester logtag

    logd ("main start: %s %s", binary_description, manifest_version);        // manifest_version automatically set during build
    logd (copyright);                                                   // Copyright

    utils_init ();

    if (server_mode) {                                                  // If server mode...
      return (server_run ());
    }

    return (client_run ());
  }

