static 
FILE * create_dump_file(const char* prefix,my_thread_id thread_id)
{
  char dump_file_name[FN_REFLEN+1]={0},tmp_dump_filename[FN_REFLEN+1]={0};
  char *rsplit=NULL;
  strncpy(tmp_dump_filename,log_error_dest,strlen(log_error_dest));
  if((rsplit=strrchr(tmp_dump_filename,'/')))
  {
    *rsplit=0;
  }
  snprintf(dump_file_name,FN_REFLEN,"%s/%s_%u.trc",tmp_dump_filename,prefix,thread_id);
  return fopen(dump_file_name,"a+");
}
static 
void zdr_dump_mdl(FILE* fp_dump_mdl)
{
  uint thd_cnt=Global_THD_manager::get_instance()->get_thd_count();
  mysql_mutex_lock(Global_THD_manager::get_instance()->get_LOCK_thd_list());
  for(uint i=0;i<thd_cnt;i++)
  {
    THD* tmp_thd=Global_THD_manager::get_instance()->get_thd_by_idx(i);
    if(tmp_thd)
    { 
      fprintf(fp_dump_mdl,"----------------------\n"
                          "processlist_id:%u\n",tmp_thd->thread_id());
      tmp_thd->mdl_context.dump_mdl_ctx(fp_dump_mdl);
    }
  }
  mysql_mutex_unlock(Global_THD_manager::get_instance()->get_LOCK_thd_list());
}
static bool update_zdr_dump_mdl(sys_var *self, THD *thd,
                                          enum_var_type type)
{
  FILE *fp_dmp_mdl=0;
  if(!thd) return FALSE;
  if(!(fp_dmp_mdl=create_dump_file("dump_mysql_mdl",thd->thread_id())))
  {
    sql_print_error("create_dump_file error");
    return FALSE;
  }
  time_t         the_time;
  struct tm      *tm_ptr=NULL;
  time(&the_time);
  tm_ptr=localtime(&the_time);
  fprintf(fp_dmp_mdl,"**** %04d-%02d-%02d %02d:%02d:%02d \n",      
    tm_ptr->tm_year+1900,tm_ptr->tm_mon+1,tm_ptr->tm_mday,tm_ptr->tm_hour,tm_ptr->tm_min,tm_ptr->tm_sec);
  zdr_dump_mdl(fp_dmp_mdl);
  fclose(fp_dmp_mdl);
  return false;
}       
static Sys_var_mybool Sys_zdr_dump_mdl(
       "zdr_dump_mdl",
       "dump system MDL",
       SESSION_VAR(zdr_dump_mdl),
       CMD_LINE(OPT_ARG),
       DEFAULT(FALSE), NO_MUTEX_GUARD, NOT_IN_BINLOG, ON_CHECK(0),
       ON_UPDATE(update_zdr_dump_mdl));