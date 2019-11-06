/***********************************************
**get space_id by rseg id*/
ulint 
trx_undo_get_space_id
(
 const ulint rseg_id
)
{
  return((trx_sys->rseg_array[rseg_id])?(trx_sys->rseg_array[rseg_id])->space:0);
}