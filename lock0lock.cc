/**dump record lock*/
void 
lock_trx_dump_reclock(
FILE*  fp_dump_reclock,
const lock_t*  lock
)
{
    ulint            space;
    ulint            page_no;
    mtr_t            mtr;
    space = lock->un_member.rec_lock.space;
    page_no = lock->un_member.rec_lock.page_no;
    mtr_start(&mtr);
    const buf_block_t*    block;
    block = buf_page_try_get(page_id_t(space, page_no), &mtr);
    fprintf(fp_dump_reclock, "RECORD LOCK: index %s of table %s.\n"
                             "  space_id:   %lu \n"
                             "  page_no:    %lu \n"
                             "  n recs:     %lu \n"
                             "  n bits:     %lu \n",
                 lock->index->name(),
                 lock->index->table_name,
                 (ulong) space, (ulong) page_no,
                 page_get_n_recs(buf_block_get_frame(block)),
        (ulong) lock_rec_get_n_bits(lock)); 
    fprintf(fp_dump_reclock, "  mode:%u  (0-IS 1=IX 2-S 3-X 4-AUTO_INC 5-NONE)\n"
                             "  type:%ld  (0-ORDINARY 256-WAIT 512-GAP 1024-REC_NOT_GAP 2048-INSERT_INTENTION)\n",
                             lock_get_mode(lock),
                             (lock->type_mode&0xF00UL));
    if(lock_get_wait(lock))
    {
      trx_t *trx=lock->trx;
      const lock_t*        curr_lock;
      const lock_t*        prev_lock=lock;
      ulint         wait_lock_heap_no;
      wait_lock_heap_no= lock_rec_find_set_bit(lock);
      fprintf(fp_dump_reclock,"  waiting_rec_lock %lu:%lu:%lu:%lu \n",trx->id,space,page_no,wait_lock_heap_no);
      lock_mutex_enter();
      for (curr_lock = lock_rec_get_prev(prev_lock,wait_lock_heap_no);
             curr_lock != NULL;
             curr_lock = lock_rec_get_prev(prev_lock,wait_lock_heap_no)) 
      {
            if (lock_has_to_wait(lock,curr_lock)) 
            {
               fprintf(fp_dump_reclock,"  blocking_rec_lock %lu:%lu:%lu:%lu \n",curr_lock->trx->id,space,page_no,wait_lock_heap_no);
            }
            prev_lock=curr_lock;
      }
      lock_mutex_exit();
    }
    fprintf(fp_dump_reclock,"  heap_no bits [ptr=%p]:\n""  ",&lock[1]);
    for (ulint i = 0; i < lock_rec_get_n_bits(lock); ) 
    {
        fprintf(fp_dump_reclock,"%lu",lock_rec_get_nth_bit(lock, i));
        ++i;
        if(0==i%8) fprintf(fp_dump_reclock,"  ");
        if(0==i%64) fprintf(fp_dump_reclock,"\n""  ");
    }
    fprintf(fp_dump_reclock,"\n");
    mtr_commit(&mtr);
    return;
}
/*****************************************************************/
/**dump table lock*/
static 
void 
lock_trx_dump_tablelock(
FILE*  fp_dump_tablock,
const lock_t*  lock
)
{
    fprintf(fp_dump_tablock, "TABLE LOCK table %s \n",lock->un_member.tab_lock.table->name.m_name);
    fprintf(fp_dump_tablock, "  mode:%d  (0-IS 1=IX 2-S 3-X 4-AUTO_INC 5-NONE)\n",
                             lock_get_mode(lock)); 
    if(lock_get_wait(lock))
    {
      trx_t *trx=lock->trx;
      const lock_t*        curr_lock;
      const lock_t*        prev_lock=lock;
      fprintf(fp_dump_tablock,"  waiting_table_lock %lu:%s \n",trx->id,lock->un_member.tab_lock.table->name.m_name);
      for (curr_lock = UT_LIST_GET_PREV(un_member.tab_lock.locks, prev_lock);
             curr_lock != NULL;
             curr_lock =UT_LIST_GET_PREV(un_member.tab_lock.locks, prev_lock) ) 
      {
            if (lock_has_to_wait(lock,curr_lock)) 
            {
               fprintf(fp_dump_tablock,"  blocking_table_lock %lu:%s \n",curr_lock->trx->id,lock->un_member.tab_lock.table->name.m_name);
            }
            prev_lock=curr_lock;
      }
    }
}
/*****************************************************************/
/**dump trx locks*/
void
lock_trx_dump_trxlock(
FILE*  fp_dump_trxlock,
const trx_t*  trx
)
{ 
  fprintf(fp_dump_trxlock,"----------------------------------------------\n"
   "Trx_id %ld has %ld lock objects\n",trx->id,UT_LIST_GET_LEN(trx->lock.trx_locks));
  const lock_t* lock;
  ulint   i = 0;
  for (lock = UT_LIST_GET_FIRST(trx->lock.trx_locks);
       lock != NULL ;
       lock = UT_LIST_GET_NEXT(trx_locks, lock), ++i) 
  {
    fprintf(fp_dump_trxlock,"-------------------\n"
    " %ld th lock object [ptr=%p][size=%ld].\n",i,lock,sizeof(*lock));
    if (lock_get_type_low(lock) == LOCK_REC)
    {
      lock_trx_dump_reclock(fp_dump_trxlock,lock);
    }
    else
    {
      lock_trx_dump_tablelock(fp_dump_trxlock,lock);
    }
  }
  fprintf(fp_dump_trxlock,"----------------------------------------------\n");
  return;  
}
/*****************************************************************/
/**dump trx locks*/
void
lock_trx_dump_trxslocks(
FILE*  fp_dump_trxslocks)
{
  const trx_t*	trx;
  for (trx = UT_LIST_GET_FIRST(trx_sys->mysql_trx_list);
	   trx != NULL ;
       trx = UT_LIST_GET_NEXT(mysql_trx_list, trx))
  {
     lock_trx_dump_trxlock(fp_dump_trxslocks,trx);
  }
}