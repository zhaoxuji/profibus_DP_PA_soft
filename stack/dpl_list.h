/**************************************** Copyright (c) **************************************************
**--File Info---------------------------------------------------------------------------------------------
**  File name          : dp_list.h
**  Last modified Date :
**  Last Version       : V1.0
**  Descriptions       : macros for double pointered list
**
*********************************************************************************************************/

/*****************************************************************************/
/* contents:

    dpl_init_list__                  (list_ptr)

    dpl_list_empty__                 (list_ptr)
    dpl_list_not_empty__             (list_ptr)

    dpl_put_blk_to_list_start__      (list_ptr, blk_ptr)
    dpl_put_blk_to_list_end__        (list_ptr, blk_ptr)
    dpl_put_blk_queue_to_list_end__  (list_ptr, blk_start_ptr, blk_end_ptr)

    dpl_get_blk_from_list__          (list_ptr, adr_blk_ptr)
    dpl_remove_blk_from_list__       (list_ptr)
    dpl_get_remove_blk_from_list__   (list_ptr, adr_blk_ptr)
    dpl_remove_blk__                 (blk_ptr)

*/
/*****************************************************************************/
/* reinclude protection */


#ifndef DPLLIST_H
#define DPLLIST_H


/*****************************************************************************/
/*  CHECK PARAMETER                                                          */
/*****************************************************************************/
#define DPL_PTR_ATTR
#define DPL_HOST_PTR_COMPARE_TYPE   void *

#ifdef  DPL_PTR_ATTR

#define _PACKED_    __packed

/*****************************************************************************/
/*  ANCHOR                                                                   */
/*****************************************************************************/

typedef struct dpl_list_cb
{
	void* resv;
    struct dpl_list_cb DPL_PTR_ATTR *next_blk_ptr;
    struct dpl_list_cb DPL_PTR_ATTR *prev_blk_ptr;

} _PACKED_  DPL_STRUC_LIST_CB;

#define DPL_STRUC_LIST_CB_PTR            DPL_STRUC_LIST_CB DPL_PTR_ATTR *

/*****************************************************************************/
/*  CASTS                                                                    */
/*****************************************************************************/


#define DPL_LIST_CB_PTR                                                       \
        DPL_STRUC_LIST_CB DPL_PTR_ATTR *

#define DPL_LIST_CB_PTR_PTR                                                   \
        DPL_STRUC_LIST_CB DPL_PTR_ATTR * DPL_PTR_ATTR *


/*****************************************************************************/
/*  LIST HANDLING                                                            */
/*****************************************************************************/


/*****************************************************************************/
/*                                                                           */
/*                           double pointered list                           */
/*                                                                           */
/*         +-------------------------------------------------------+         */
/*         |                                                       |         */
/*         |   list-anchor                                         |         */
/*         |    +------+     +------+     +------+     +------+    |         */
/*         +--> | next | --> | next | --> | next | ... | next | ---+         */
/*         +--- | prev | <-- | prev | <-- | prev | ... | prev | <--+         */
/*         |    +------+     | ...  |     | ...  |     | ...  |    |         */
/*         |                 +------+     +------+     +------+    |         */
/*         |                                                       |         */
/*         +-------------------------------------------------------+         */
/*                                                                           */
/*****************************************************************************/


/*****************************************************************************/
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  initialize desired double pointered list.                                */
/*                                                                           */
/*****************************************************************************/
#define dpl_init_list__(list_ptr)                                             \
{                                                                             \
  /* list.next = list.prev = list */                                          \
  ((DPL_LIST_CB_PTR) (list_ptr)) -> next_blk_ptr                              \
  = ((DPL_LIST_CB_PTR) (list_ptr)) -> prev_blk_ptr                            \
  = (DPL_LIST_CB_PTR) (list_ptr);                                             \
}                                                                             \


/*****************************************************************************/
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  check if desired double pointered list is empty.                         */
/*                                                                           */
/*****************************************************************************/


#define dpl_list_empty__(list_ptr)                                            \
(                                                                             \
  /* list.next == list */                                                     \
  ((DPL_HOST_PTR_COMPARE_TYPE)                                                \
  (((DPL_LIST_CB_PTR) (list_ptr)) -> next_blk_ptr))                           \
  ==                                                                          \
  ((DPL_HOST_PTR_COMPARE_TYPE)                                                \
  (list_ptr))                                                                 \
)


/*****************************************************************************/
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  check if desired double pointered list is not empty.                     */
/*                                                                           */
/*****************************************************************************/


#define dpl_list_not_empty__(list_ptr)                                        \
(                                                                             \
  /* list.next != list */                                                     \
  ((DPL_HOST_PTR_COMPARE_TYPE)                                                \
  (((DPL_LIST_CB_PTR) (list_ptr)) -> next_blk_ptr))                           \
  !=                                                                          \
  ((DPL_HOST_PTR_COMPARE_TYPE)                                                \
  (list_ptr))                                                                 \
)


/*****************************************************************************/
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  put block to start of double pointered list.                             */
/*                                                                           */
/*****************************************************************************/


#define dpl_put_blk_to_list_start__(list_ptr, blk_ptr)                        \
{                                                                             \
  /* list.next -> prev = blk */                                               \
  ((DPL_LIST_CB_PTR) (list_ptr)) -> next_blk_ptr -> prev_blk_ptr              \
  = (DPL_LIST_CB_PTR) (blk_ptr);                                              \
                                                                              \
  /* blk.next = list.next */                                                  \
  ((DPL_LIST_CB_PTR) (blk_ptr)) -> next_blk_ptr                               \
  = ((DPL_LIST_CB_PTR) (list_ptr)) -> next_blk_ptr;                           \
                                                                              \
  /* list.next = blk */                                                       \
  ((DPL_LIST_CB_PTR) (list_ptr)) -> next_blk_ptr                              \
  = (DPL_LIST_CB_PTR) (blk_ptr);                                              \
                                                                              \
  /* blk.prev = list */                                                       \
  ((DPL_LIST_CB_PTR) (blk_ptr)) -> prev_blk_ptr                               \
  = (DPL_LIST_CB_PTR) (list_ptr);                                             \
}                                                                             \


/*****************************************************************************/
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  put block to end of double pointered list.                               */
/*                                                                           */
/*****************************************************************************/


#define dpl_put_blk_to_list_end__(list_ptr, blk_ptr)                          \
{                                                                             \
  /* blk.next = list */                                                       \
  ((DPL_LIST_CB_PTR) (blk_ptr)) -> next_blk_ptr                               \
  = (DPL_LIST_CB_PTR) (list_ptr);                                             \
                                                                              \
  /* list.prev -> next = blk */                                               \
  ((DPL_LIST_CB_PTR) (list_ptr)) -> prev_blk_ptr -> next_blk_ptr              \
  = (DPL_LIST_CB_PTR) (blk_ptr);                                              \
                                                                              \
  /* blk.prev = list.prev */                                                  \
  ((DPL_LIST_CB_PTR) (blk_ptr)) -> prev_blk_ptr                               \
  = ((DPL_LIST_CB_PTR) (list_ptr)) -> prev_blk_ptr;                           \
                                                                              \
  /* list.prev = blk */                                                       \
  ((DPL_LIST_CB_PTR) (list_ptr)) -> prev_blk_ptr                              \
  = (DPL_LIST_CB_PTR) (blk_ptr);                                              \
}                                                                             \


/*****************************************************************************/
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  put queue of blocks to end of double pointered list.                     */
/*                                                                           */
/*****************************************************************************/


#define dpl_put_blk_queue_to_list_end__(list_ptr, blk_start_ptr, blk_end_ptr)\
{                                                                             \
  /* list.prev -> next = first blk */                                         \
  ((DPL_LIST_CB_PTR) (list_ptr)) -> prev_blk_ptr -> next_blk_ptr              \
  = (DPL_LIST_CB_PTR) (blk_start_ptr);                                        \
                                                                              \
  /* first blk.prev = list.prev */                                            \
  ((DPL_LIST_CB_PTR) (blk_start_ptr)) -> prev_blk_ptr                         \
  = ((DPL_LIST_CB_PTR) (list_ptr)) -> prev_blk_ptr;                           \
                                                                              \
  /* list.prev = last blk */                                                  \
  ((DPL_LIST_CB_PTR) (list_ptr)) -> prev_blk_ptr                              \
  = (DPL_LIST_CB_PTR) (blk_end_ptr);                                          \
                                                                              \
  /* last blk.next = list */                                                  \
  ((DPL_LIST_CB_PTR) (blk_end_ptr)) -> next_blk_ptr                           \
  = (DPL_LIST_CB_PTR) (list_ptr);                                             \
}                                                                             \


/*****************************************************************************/
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  get block from start of desired double pointered list.                   */
/*                                                                           */
/*****************************************************************************/


#define dpl_get_blk_from_list__(list_ptr, adr_blk_ptr)                        \
{                                                                             \
  /* blk = list.next */                                                       \
  *((DPL_LIST_CB_PTR_PTR) (adr_blk_ptr))                                      \
  = ((DPL_LIST_CB_PTR) (list_ptr)) -> next_blk_ptr;                           \
}                                                                             \


/*****************************************************************************/
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  remove block from start of desired double pointered list.                */
/*                                                                           */
/*****************************************************************************/


#define dpl_remove_blk_from_list__(list_ptr)                                  \
{                                                                             \
  /* list.next = list.next -> next */                                         \
  ((DPL_LIST_CB_PTR) (list_ptr)) -> next_blk_ptr                              \
  = ((DPL_LIST_CB_PTR) (list_ptr)) -> next_blk_ptr -> next_blk_ptr;           \
                                                                              \
  /* list.next -> prev = list */                                              \
  ((DPL_LIST_CB_PTR) (list_ptr)) -> next_blk_ptr -> prev_blk_ptr              \
  = (DPL_LIST_CB_PTR) (list_ptr);                                             \
}                                                                             \


/*****************************************************************************/
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  get and remove block from start of desired double pointered list.        */
/*                                                                           */
/*****************************************************************************/


#define dpl_get_remove_blk_from_list__(list_ptr, adr_blk_ptr)                 \
{                                                                             \
  dpl_get_blk_from_list__((list_ptr), (adr_blk_ptr));                         \
                                                                              \
  dpl_remove_blk_from_list__(list_ptr);                                       \
}                                                                             \


/*****************************************************************************/
/*  D e s c r i p t i o n :                                                  */
/*                                                                           */
/*  remove block from anywhere inside a double pointered list.               */
/*                                                                           */
/*****************************************************************************/


#define dpl_remove_blk__(blk_ptr)                                             \
{                                                                             \
  /* blk.prev -> next = blk.next */                                           \
  ((DPL_LIST_CB_PTR) (blk_ptr)) -> prev_blk_ptr -> next_blk_ptr               \
  = ((DPL_LIST_CB_PTR) (blk_ptr)) -> next_blk_ptr;                            \
                                                                              \
  /* blk.next -> prev = blk.prev */                                           \
  ((DPL_LIST_CB_PTR) (blk_ptr)) -> next_blk_ptr -> prev_blk_ptr               \
  = ((DPL_LIST_CB_PTR) (blk_ptr)) -> prev_blk_ptr;                            \
}                                                                             \


/*****************************************************************************/
/* parameter check */


#else
    #error dplist handler: no list handling data attribute DPL_PTR_ATTR defined !
#endif


/*****************************************************************************/
/* reinclude-protection */


#else
    #pragma message ("The header DPLLIST.H is included twice or more !")
#endif


