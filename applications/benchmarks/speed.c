#include <sys/times.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include "zmw/zmw.h"
#include "zmw/zmw_private.h"

void vertical(int depth) ;

static int *global_nb ;
static int global_depth ;
static int global_children ;
static int global_focus ;
static char *global_text ;

#ifndef ZMW_VERSION_MAJOR
#define zmw_fixed zmw_box
#define zmw_hbox zmw_box_horizontal
#define zmw_vbox zmw_box_vertical
#define zmw_main zmw_run
#define zmw_label zmw_text
#define zmw_child_cache_size zmw_cache_size

#else

#if ZMW_VERSION_MAJOR == 0 && ZMW_VERSION_MINOR < 2
#define zmw_child_cache_size zmw_cache_size
#endif

#endif

#ifndef zmw_action_get
#define zmw_action_get() ZMW_ACTION
#endif
#ifndef zmw_focus_get
#define zmw_focus_get() ZMW_FOCUS
#endif



void leaf()
{
  zmw_width(1) ;
  zmw_height(1) ;
  if ( global_focus )
    ZMW(zmw_decorator(Zmw_Decorator_Focusable))
    {
      if ( global_text )
	zmw_label(global_text) ;
    }
  else
    ZMW(zmw_fixed())
    {
      if ( global_text )
	zmw_label(global_text) ;
    }
  (*global_nb)++ ;
}


void horizontal(int depth)
{
  int i ;

  if ( depth == 0 )
    {
      leaf() ;
    }
  else
    ZMW(zmw_hbox())
    {
      for(i=0; i<global_children; i++)
	vertical(depth-1) ;
    }
}

void vertical(int depth)
{
  int i ;

  if ( depth == 0 )
    {
      leaf() ;
    }
  else
    ZMW(zmw_vbox())
    {
      for(i=0; i<global_children; i++)
	horizontal(depth-1) ;
    }
}

void get_value(char *buf, char *key)
{
  if ( strncmp(buf, key, strlen(key)) == 0)
    {
      printf("%s=%d\n", key, atoi( strchr(buf, ':')+1 )) ;
    }
}

void get_mem_usage()
{
  FILE *f ;
  char buf[999] ;


  f = fopen("/proc/self/status", "r") ;
  for(;;)
    {
      if ( fgets(buf, sizeof(buf), f) == NULL )
	break ;
      
      get_value(buf, "VmExe") ;
      get_value(buf, "VmStk") ;
      get_value(buf, "VmLib") ;
      get_value(buf, "VmData") ;
    }
  fclose(f) ;
}



#define ACTION(A) { A, #A }


void many()
{
  static struct
  {
    int (*action)(void) ;	/* Action timed */
    char *action_name ;
    int time ;			/* In number of click */
    int nb ;			/* Number of call */
    int nb_text_eval ;
  } actions[7] = {
    ACTION(zmw_action_compute_required_size),
    ACTION(zmw_action_draw),
    ACTION(zmw_action_dispatch_event),
    ACTION(zmw_action_search),
    ACTION(zmw_action_dispatch_accelerator),
    ACTION(zmw_action_clean),
    {NULL},
  } ;
  static int clk = 0 ;		/* Number of click per second */
  static int exiting = 0 ;

  int i, j, nb_text ;
  struct tms begin_tms, end_tms ;
  char tmp[999] ;

  if ( global_focus && clk == 0 )
    {
      for(i=0; i<200; i++)
	{
	  strcpy(tmp, "") ;
	  for(j=0; j<10; j++)
	    sprintf(tmp+strlen(tmp), "/.%d", rand()%10) ;
	  zmw_name_set_value_int_with_name(tmp, "?", 0) ;
	}
      zmw_name_register_with_name(zmw_focus_get(), "/.0/.0/.0/.0") ;
    }

  if ( clk == 0 )
    clk = sysconf(_SC_CLK_TCK);

  i=0 ;
  while ( actions[i].action != zmw_action_get() )
    i++ ;
  global_nb = &actions[i].nb_text_eval ;

  times(&begin_tms) ;

  zmw_padding_width(0) ;
  zmw_focus_width(0) ;
  ZMW(zmw_window("Title"))
    vertical(global_depth) ;

  times(&end_tms) ;

  actions[i].time += end_tms.tms_utime - begin_tms.tms_utime ;
  actions[i].nb++ ;


  if ( zmw_action_get() == zmw_action_draw )
    {
      creat("xxx.drawdone", 0700) ;
    }


  
  if ( zmw_action_get() == zmw_action_dispatch_event )
    {
      if ( exiting )
	return ;
      exiting = 1 ;

      nb_text = rint(pow(global_children, global_depth)) ;

      printf("cache size=%d\n", zmw_child_cache_size()) ;

#ifdef ZMW_DEBUG_INSIDE_ZMW_PARAMETER
      printf("ZMW_DEBUG_INSIDE_ZMW_PARAMETER=%d\n", ZMW_DEBUG_INSIDE_ZMW_PARAMETER) ;
#endif
#ifdef ZMW_DEBUG_NAME
      printf("ZMW_DEBUG_NAME=%d\n", ZMW_DEBUG_NAME) ;
      printf("ZMW_DEBUG_ASSERT=%d\n", ZMW_DEBUG_ASSERT) ;
      printf("ZMW_DEBUG_STORE_WIDGET_TYPE=%d\n", ZMW_DEBUG_STORE_WIDGET_TYPE) ;
#endif
      printf("#children=%d\n", global_children) ;
      printf("#leaf_widget=%d\n", nb_text) ;
      get_mem_usage() ;
      for(i=0; actions[i].action; i++)
	{
	  if ( actions[i].nb )
	    {
	      printf("%s.#leaf_eval=%d\n"
		     , actions[i].action_name
		     , actions[i].nb_text_eval/actions[i].nb/nb_text
		     ) ;
	      printf("%s.#nb_samples=%d\n"
		     , actions[i].action_name
		     , actions[i].nb
		     ) ;
	      printf("%s.#time=%.2f\n"
		     , actions[i].action_name
		     , actions[i].time/(float)clk/actions[i].nb
		   ) ;
	    }
	}
      printf("\n") ;
      fflush(stdout) ;
      creat("xxx.exit", 0700) ;
      exit(0) ; // Faster than zmw_main_quit(0) ;
    }

}

int main(int argc, char *argv[])
{
  int n ;
  double depth, epsilon ;

  zmw_init(&argc, &argv) ;

  global_children = 2 ;

  if ( argc > 1 )
    global_depth = atoi(argv[1]) ;
  else
    {
      fprintf(stderr, "Needs widget tree depth\n") ;
      exit(1) ;
    }

  if ( argc > 2 )
    global_children = atoi(argv[2]) ;
  else
    {
      fprintf(stderr, "Needs number of children per node\n") ;
      exit(1) ;
    }
  
  if ( argc > 3 )
    global_focus = atoi(argv[3]) ;
  else
    global_focus = 0 ;

  if ( argc > 4 )
    global_text = argv[4] ;
  else
    global_text = NULL ;

  n = 1 << global_depth ;
  depth = global_depth / ( log(global_children) / log(2) ) ;
  epsilon = rint(depth) - depth ;
  if ( epsilon < -0.0001 || epsilon > 0.0001 )
    {
      fprintf(stderr, "Epsilon = %g\n", epsilon) ; 
      creat("xxx.drawdone", 0700) ;
      creat("xxx.exit", 0700) ;
      return 0 ;
    }

  global_depth = rint(depth) ;
     


  zmw_main(many) ;
  return 0 ;
}


