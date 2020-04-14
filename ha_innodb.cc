/**************************************************************/
static 
FILE *
create_dump_file(const char*prefix,uint space_id,uint index_id)
{
  char dump_file_name[FN_REFLEN+1]={0},tmp_dump_filename[FN_REFLEN+1]={0};
  char *rsplit=NULL;
  strncpy(tmp_dump_filename,log_error_dest,strlen(log_error_dest));  
  if((rsplit=strrchr(tmp_dump_filename,'/')))
  {
    *rsplit=0;
  }
  snprintf(dump_file_name,FN_REFLEN,"%s/%s_%u_%u.trc",tmp_dump_filename,prefix,space_id,index_id);
  return fopen(dump_file_name,"a+");
}
static 
FILE *
create_dump_file_lsn(const char*prefix,ulint space_id,ulint index_id)
{
  char dump_file_name[FN_REFLEN+1]={0},tmp_dump_filename[FN_REFLEN+1]={0};
  char *rsplit=NULL;
  strncpy(tmp_dump_filename,log_error_dest,strlen(log_error_dest));  
  if((rsplit=strrchr(tmp_dump_filename,'/')))
  {
    *rsplit=0;
  }
  snprintf(dump_file_name,FN_REFLEN,"%s/%s_%ld_%ld.trc",tmp_dump_filename,prefix,space_id,index_id);
  return fopen(dump_file_name,"a+");
}
/****************************************************************//**
Update the system variable innodb_zdr_dump_indextree using the "saved"
value. This function is registered as a callback with MySQL. */
static
void
innodb_zdr_dump_indextree_update(
    THD*                thd,        /*!< in: thread handle */
    struct st_mysql_sys_var*    var,        /*!< in: pointer to
                            system variable */
    void*                var_ptr,    /*!< out: where the
                            formal string goes */
    const void*            save)        /*!< in: immediate result
                            from check function */
{

   index_id_t index_id=*static_cast<const index_id_t*>(save);
   dict_index_t* index=dict_index_get_if_in_cache(index_id);
   if(NULL==index)
   {
     sql_print_error("The index_id %ld not in cache",index_id);
     return;
   }
   FILE *fp_dmp_tree=0;
   if(!(fp_dmp_tree=create_dump_file("dump_indextree",dict_index_get_space(index),index_id)))
   {
     sql_print_error("create_dump_file error");
     return;
   }
   time_t         the_time;
   struct tm      *tm_ptr=NULL;
   struct timeval  tv;
   time(&the_time);
   tm_ptr=localtime(&the_time);
   gettimeofday(&tv, NULL);
   fprintf(fp_dmp_tree,"**** %04d-%02d-%02d %02d:%02d:%02d.%03ld \n",
           tm_ptr->tm_year+1900,tm_ptr->tm_mon+1,tm_ptr->tm_mday,tm_ptr->tm_hour,tm_ptr->tm_min,tm_ptr->tm_sec,tv.tv_usec);
   fprintf(fp_dmp_tree,"space_id=%ld table_name=%s index_name=%s index_id=%ld \n",
     dict_index_get_space(index),index->table_name,(const char*)(index->name),index_id);
   btr_cur_dump_index_prefetch(index,fp_dmp_tree);
   fclose(fp_dmp_tree);
}
/***************************************************************//**
Check if it is a valid cmd format. This function is registered as
a callback with MySQL.
@return 0 for valid cmd format */
static
int
innodb_zdr_dump_indexnode_validate(
/*=============================*/
  THD*                thd,    /*!< in: thread handle */
        struct st_mysql_sys_var*    var,    /*!< in: pointer to system
                            variable */
        void*                save,    /*!< out: immediate result
                            for update function */
        struct st_mysql_value*        value)    /*!< in: incoming string */
 {
    const char*    cmd_format_input;
    char        buff[64*3]={0};
    int        len = sizeof(buff);

    ut_a(save != NULL);
    ut_a(value != NULL);

    cmd_format_input = value->val_str(value, buff, &len);

    if (cmd_format_input != NULL) 
    {
        uint   splitcnt=0;
        const char     *bgnp=NULL,*endp=NULL;
        bgnp=cmd_format_input;
        endp=bgnp+strlen(cmd_format_input);
        while(*bgnp && bgnp<endp)
        {
           while(' '==*bgnp || '\t'==*bgnp)bgnp++;
           while('0'<=*bgnp && *bgnp<='9')bgnp++;
           while(' '==*bgnp || '\t'==*bgnp)bgnp++;
           if(','==*bgnp)
           {
            splitcnt++;
            bgnp++;
            continue;
           }
           else if('\0'==*bgnp)
           {
             break;
           }
           push_warning(thd, Sql_condition::SL_WARNING,
                            HA_ERR_WRONG_COMMAND, "zdr_dump_page value is invaild.zdr_dump_page=space_id,pageno,index_id");
             return(1);    
        }
        if(splitcnt!= 1)
        {
           push_warning(thd, Sql_condition::SL_WARNING,
                     HA_ERR_WRONG_COMMAND, "zdr_dump_page value format is invaild.zdr_dump_page=space_id,pageno,index_id");
           return(1);
        }
        *static_cast<const char**>(save) = cmd_format_input;
        return (0);
    }

    *static_cast<const char**>(save) = NULL;
    return(1);
 }
/****************************************************************//**
Update the system variable innodb_zdr_dump_page using the "saved"
value. This function is registered as a callback with MySQL. */
static
void
innodb_zdr_dump_indexnode_update(
    THD*                thd,        /*!< in: thread handle */
    struct st_mysql_sys_var*    var,        /*!< in: pointer to
                            system variable */
    void*                var_ptr,    /*!< out: where the
                            formal string goes */
    const void*            save)        /*!< in: immediate result
                            from check function */
{
   const char*   cmd_format=NULL;
   char  *splitch=NULL,*endptr=NULL,*bgnch=NULL;
   char  dump_format[64*3]={0};
   uint space_id=0,page_no=0;
   cmd_format = *static_cast<const char*const*>(save);
   if(cmd_format)
   {
     uint formatlen=strlen(cmd_format);
     memcpy(dump_format,cmd_format, formatlen);
     bgnch=dump_format;
     if((splitch=strchr(bgnch,',')))
     {
       *splitch=0;
       space_id=strtoull(bgnch,&endptr,10);
       bgnch=splitch+1;
     }
     page_no=strtoull(bgnch,&endptr,10);
     FILE *fp_dmp_page=0;
     if(!(fp_dmp_page=create_dump_file("dump_page",space_id,page_no)))
     {
       sql_print_error("create_dump_file error");
       return;
     }
     time_t         the_time;
     struct tm      *tm_ptr=NULL;
     struct timeval  tv;
     time(&the_time);
     tm_ptr=localtime(&the_time);
     gettimeofday(&tv, NULL);
     fprintf(fp_dmp_page,"**** %04d-%02d-%02d %02d:%02d:%02d.%03ld \n",
             tm_ptr->tm_year+1900,tm_ptr->tm_mon+1,tm_ptr->tm_mday,tm_ptr->tm_hour,tm_ptr->tm_min,tm_ptr->tm_sec,tv.tv_usec);
     btr_cur_dump_index_record(space_id,page_no,fp_dmp_page);
     fclose(fp_dmp_page);
   }
   *static_cast<const char**>(var_ptr)=cmd_format;   
   return;
}
/****************************************************************//**
Update the system variable innodb_zdr_dump_page using the "saved"
value. This function is registered as a callback with MySQL. */
static
void
innodb_zdr_dump_fsp_hdr_update(
    THD*                thd,    /*!< in: thread handle */
          struct st_mysql_sys_var*    var,    /*!< in: pointer to system
                              variable */
          void*                save,    /*!< out: immediate result
                              for update function */
          const void*        value)    /*!< in: incoming string */
{
     uint space_id=*static_cast<const uint*>(value);
     FILE *fp_dmp_fsphdr=0;
     if(!(fp_dmp_fsphdr=create_dump_file("dump_fsp_hdr",space_id,0)))
     {
       sql_print_error("create_dump_file error");
       return;
     }
     time_t         the_time;
     struct tm      *tm_ptr=NULL;
     struct timeval  tv;
     time(&the_time);
     tm_ptr=localtime(&the_time);
     gettimeofday(&tv, NULL);
     fprintf(fp_dmp_fsphdr,"**** %04d-%02d-%02d %02d:%02d:%02d.%03ld \n",
             tm_ptr->tm_year+1900,tm_ptr->tm_mon+1,tm_ptr->tm_mday,tm_ptr->tm_hour,tm_ptr->tm_min,tm_ptr->tm_sec,tv.tv_usec);
     fsp_header_dump_fsp_hdr(space_id,0,4,fp_dmp_fsphdr);
     fprintf(fp_dmp_fsphdr,"--------------------------------\n");
     fclose(fp_dmp_fsphdr);
     return;
}
/****************************************************************//**
Update the system variable innodb_zdr_dump_page using the "saved"
value. This function is registered as a callback with MySQL. */
static
void
innodb_zdr_dump_fseg_inode_update(
    THD*                thd,        /*!< in: thread handle */
    struct st_mysql_sys_var*    var,        /*!< in: pointer to
                            system variable */
    void*                var_ptr,    /*!< out: where the
                            formal string goes */
    const void*            save)        /*!< in: immediate result
                            from check function */
{
   const char*   cmd_format=NULL;
   char  *splitch=NULL,*endptr=NULL,*bgnch=NULL;
   char  dump_format[64*3]={0};
   uint space_id=0,page_no=0;
   cmd_format = *static_cast<const char*const*>(save);
   if(cmd_format)
   {
     uint formatlen=strlen(cmd_format);
     memcpy(dump_format,cmd_format, formatlen);
     bgnch=dump_format;
     if((splitch=strchr(bgnch,',')))
     {
       *splitch=0;
       space_id=strtoull(bgnch,&endptr,10);
       bgnch=splitch+1;
     }
     page_no=strtoull(bgnch,&endptr,10);
     FILE *fp_dmp_fseg=0;
     if(!(fp_dmp_fseg=create_dump_file("dump_fseg_inode",space_id,page_no)))
     {
       sql_print_error("create_dump_file error");
       return;
     }
     time_t         the_time;
     struct tm      *tm_ptr=NULL;
     struct timeval  tv;
     time(&the_time);
     tm_ptr=localtime(&the_time);
     gettimeofday(&tv, NULL);
     fprintf(fp_dmp_fseg,"**** %04d-%02d-%02d %02d:%02d:%02d.%03ld \n",
             tm_ptr->tm_year+1900,tm_ptr->tm_mon+1,tm_ptr->tm_mday,tm_ptr->tm_hour,tm_ptr->tm_min,tm_ptr->tm_sec,tv.tv_usec);
     fsp_header_dump_fseg_page(space_id,page_no,fp_dmp_fseg);
     fprintf(fp_dmp_fseg,"--------------------------------\n");
     fclose(fp_dmp_fseg);
   }
   *static_cast<const char**>(var_ptr)=cmd_format;
   return;
}

/****************************************************************//**
Update the system variable innodb_zdr_dump_undo_architecture using the "saved"
value. This function is registered as a callback with MySQL. */
static
void
innodb_zdr_dump_undo_architecture(
    THD*                thd,    /*!< in: thread handle */
          struct st_mysql_sys_var*    var,    /*!< in: pointer to system
                              variable */
          void*                save,    /*!< out: immediate result
                              for update function */
          const void*        value)    /*!< in: incoming string */
{
    FILE *fp_dmp_undo_arch=0;
    if(!(fp_dmp_undo_arch=create_dump_file("dump_undo_architecture",0,5)))
    {
      sql_print_error("create_dump_file error");
      return;
    }
    time_t         the_time;
    struct tm      *tm_ptr=NULL;
    struct timeval  tv;
    time(&the_time);
    tm_ptr=localtime(&the_time);
    gettimeofday(&tv, NULL);
    fprintf(fp_dmp_undo_arch,"**** %04d-%02d-%02d %02d:%02d:%02d.%03ld \n",
            tm_ptr->tm_year+1900,tm_ptr->tm_mon+1,tm_ptr->tm_mday,tm_ptr->tm_hour,tm_ptr->tm_min,tm_ptr->tm_sec,tv.tv_usec);
    btr_cur_dump_undo_architecture(0,TRX_SYS_PAGE_NO,fp_dmp_undo_arch);
    fclose(fp_dmp_undo_arch);
    return;
}

/****************************************************************//**
Update the system variable innodb_zdr_dump_rseg_node using the "saved"
value. This function is registered as a callback with MySQL. */
static
void
innodb_zdr_dump_rseg_node_update(
    THD*                thd,        /*!< in: thread handle */
    struct st_mysql_sys_var*    var,        /*!< in: pointer to
                            system variable */
    void*                var_ptr,    /*!< out: where the
                            formal string goes */
    const void*            save)        /*!< in: immediate result
                            from check function */
{
   const char*   cmd_format=NULL;
   char  *splitch=NULL,*endptr=NULL,*bgnch=NULL;
   char  dump_format[64*3]={0};
   uint space_id,page_no;
   cmd_format = *static_cast<const char*const*>(save);
   if(cmd_format)
   {
     uint formatlen=strlen(cmd_format);
     memcpy(dump_format,cmd_format, formatlen);
     bgnch=dump_format;
     if((splitch=strchr(bgnch,',')))
     {
       *splitch=0;
       space_id=strtoull(bgnch,&endptr,10);
       bgnch=splitch+1;
     }
     page_no=strtoull(bgnch,&endptr,10);
     FILE *fp_dmp_rseg=0;
     if(!(fp_dmp_rseg=create_dump_file("dump_rseg_inode",space_id,page_no)))
     {
       sql_print_error("create_dump_file error");
       return;
     }
     time_t         the_time;
     struct tm      *tm_ptr=NULL;
     struct timeval  tv;
     time(&the_time);
     tm_ptr=localtime(&the_time);
     gettimeofday(&tv, NULL);
     fprintf(fp_dmp_rseg,"**** %04d-%02d-%02d %02d:%02d:%02d.%03ld \n",
             tm_ptr->tm_year+1900,tm_ptr->tm_mon+1,tm_ptr->tm_mday,tm_ptr->tm_hour,tm_ptr->tm_min,tm_ptr->tm_sec,tv.tv_usec);
     btr_cur_dump_rseg_inode(space_id,page_no,fp_dmp_rseg);
     fclose(fp_dmp_rseg);
   }
   *static_cast<const char**>(var_ptr)=cmd_format;
   return;
}
/****************************************************************//**
Update the system variable innodb_zdr_dump_undolog_node using the "saved"
value. This function is registered as a callback with MySQL. */
static
void
innodb_zdr_dump_undolog_node_update(
    THD*                thd,        /*!< in: thread handle */
    struct st_mysql_sys_var*    var,        /*!< in: pointer to
                            system variable */
    void*                var_ptr,    /*!< out: where the
                            formal string goes */
    const void*            save)        /*!< in: immediate result
                            from check function */
{
   const char*   cmd_format=NULL;
   char  *splitch=NULL,*endptr=NULL,*bgnch=NULL;
   char  dump_format[64*3]={0};
   uint space_id,page_no;
   cmd_format = *static_cast<const char*const*>(save);
   if(cmd_format)
   {
     uint formatlen=strlen(cmd_format);
     memcpy(dump_format,cmd_format, formatlen);
     bgnch=dump_format;
     if((splitch=strchr(bgnch,',')))
     {
       *splitch=0;
       space_id=strtoull(bgnch,&endptr,10);
       bgnch=splitch+1;
     }
     page_no=strtoull(bgnch,&endptr,10);
     FILE *fp_dmp_undolog=0;
     if(!(fp_dmp_undolog=create_dump_file("dump_undolog_inode",space_id,page_no)))
     {
       sql_print_error("create_dump_file error");
       return;
     }
     time_t         the_time;
     struct tm      *tm_ptr=NULL;
     struct timeval  tv;
     time(&the_time);
     tm_ptr=localtime(&the_time);
     gettimeofday(&tv, NULL);
     fprintf(fp_dmp_undolog,"**** %04d-%02d-%02d %02d:%02d:%02d.%03ld \n",
             tm_ptr->tm_year+1900,tm_ptr->tm_mon+1,tm_ptr->tm_mday,tm_ptr->tm_hour,tm_ptr->tm_min,tm_ptr->tm_sec,tv.tv_usec);
     btr_cur_dump_undolog_inode(space_id,page_no,fp_dmp_undolog);
     fclose(fp_dmp_undolog);
   }
   *static_cast<const char**>(var_ptr)=cmd_format;
   return;
}
/****************************************************************//**
Update the system variable innodb_zdr_dump_indextree using the "saved"
value. This function is registered as a callback with MySQL. */
static
void
innodb_zdr_dump_trxlock_update(
    THD*                thd,        /*!< in: thread handle */
    struct st_mysql_sys_var*    var,        /*!< in: pointer to
                            system variable */
    void*                var_ptr,    /*!< out: where the
                            formal string goes */
    const void*            save)        /*!< in: immediate result
                            from check function */
{
  trx_id_t  trx_id=*static_cast<const trx_id_t*>(save);
  trx_t*   trx=NULL;
  if(1!=trx_id)
  {
    for (trx = UT_LIST_GET_FIRST(trx_sys->rw_trx_list);
         trx != NULL;
         trx = UT_LIST_GET_NEXT(trx_list, trx)) 
    {
        if(trx_id==trx->id)
            break;
    }
    if(NULL==trx)
    {
       sql_print_error("The trx_id %ld does not exist",trx_id);
       return;
    }
  }
  FILE *fp_dmp_trxlock=0;
  if(!(fp_dmp_trxlock=create_dump_file("dump_trxlock",trx_id,0)))
  {
    sql_print_error("create_dump_file error");
    return;
  }
  time_t         the_time;
  struct tm      *tm_ptr=NULL;
  struct timeval  tv;
  time(&the_time);
  tm_ptr=localtime(&the_time);
  gettimeofday(&tv, NULL);
  fprintf(fp_dmp_trxlock,"**** %04d-%02d-%02d %02d:%02d:%02d.%03ld \n",
          tm_ptr->tm_year+1900,tm_ptr->tm_mon+1,tm_ptr->tm_mday,tm_ptr->tm_hour,tm_ptr->tm_min,tm_ptr->tm_sec,tv.tv_usec);
  if(1!=trx_id)
  lock_trx_dump_trxlock(fp_dmp_trxlock,trx);
  else
  lock_trx_dump_trxslocks(fp_dmp_trxlock);
  fclose(fp_dmp_trxlock);
  return;
}
/****************************************************************//**
Update the system variable innodb_zdr_dump_indextree using the "saved"
value. This function is registered as a callback with MySQL. */
static
void
innodb_zdr_dump_redo_hdr_update(
    THD*                thd,        /*!< in: thread handle */
    struct st_mysql_sys_var*    var,        /*!< in: pointer to
                            system variable */
    void*                var_ptr,    /*!< out: where the
                            formal string goes */
    const void*            save)        /*!< in: immediate result
                            from check function */
{
    
    FILE* fp_dmp_redo_loghdr=0;
    if(!(fp_dmp_redo_loghdr=create_dump_file("dump_redo_hdr",4294967280,0)))
    {
      sql_print_error("create_dump_file error");
      return;
    }
    time_t         the_time;
    struct tm      *tm_ptr=NULL;
    struct timeval  tv;
    time(&the_time);
    tm_ptr=localtime(&the_time);
    gettimeofday(&tv, NULL);
    fprintf(fp_dmp_redo_loghdr,"**** %04d-%02d-%02d %02d:%02d:%02d.%03ld \n",
            tm_ptr->tm_year+1900,tm_ptr->tm_mon+1,tm_ptr->tm_mday,tm_ptr->tm_hour,tm_ptr->tm_min,tm_ptr->tm_sec,tv.tv_usec);
    //log_print(fp_dmp_redo_loghdr);
    //FILE* tmp_fp=fp_dmp_redo_loghdr;
    log_dump_redo_loghdr(fp_dmp_redo_loghdr);
    fclose(fp_dmp_redo_loghdr);
    return;
}
/****************************************************************//**
Update the system variable innodb_zdr_dump_indextree using the "saved"
value. This function is registered as a callback with MySQL. */
static
void
innodb_zdr_dump_redo_logblock_update(
    THD*                thd,        /*!< in: thread handle */
    struct st_mysql_sys_var*    var,        /*!< in: pointer to
                            system variable */
    void*                var_ptr,    /*!< out: where the
                            formal string goes */
    const void*            save)        /*!< in: immediate result
                            from check function */
{
   const char*   cmd_format=NULL;
   char  *splitch=NULL,*endptr=NULL,*bgnch=NULL;
   char  dump_format[64*3]={0};
   ulint start_lsn,end_lsn;
   cmd_format = *static_cast<const char*const*>(save);
   if(cmd_format)
   {
     uint formatlen=strlen(cmd_format);
     memcpy(dump_format,cmd_format, formatlen);
     bgnch=dump_format;
     if((splitch=strchr(bgnch,',')))
     {
       *splitch=0;
       start_lsn=strtoull(bgnch,&endptr,10);
       bgnch=splitch+1;
     }
     end_lsn=strtoull(bgnch,&endptr,10);
     FILE *fp_dmp_redo_logblock=0;
     if(!(fp_dmp_redo_logblock=create_dump_file_lsn("dump_redo_block",start_lsn,end_lsn)))
     {
       sql_print_error("create_dump_file error");
       return;
     }
     time_t         the_time;
     struct tm      *tm_ptr=NULL;
     struct timeval  tv;
     time(&the_time);
     tm_ptr=localtime(&the_time);
     gettimeofday(&tv, NULL);
     fprintf(fp_dmp_redo_logblock,"**** %04d-%02d-%02d %02d:%02d:%02d.%03ld \n",
             tm_ptr->tm_year+1900,tm_ptr->tm_mon+1,tm_ptr->tm_mday,tm_ptr->tm_hour,tm_ptr->tm_min,tm_ptr->tm_sec,tv.tv_usec);
     btr_cur_dump_redo_block(fp_dmp_redo_logblock,start_lsn,end_lsn);
     fclose(fp_dmp_redo_logblock);
   }
   *static_cast<const char**>(var_ptr)=cmd_format;
   return;
}
/****************************************************************//**
Update the system variable innodb_zdr_dump_indextree using the "saved"
value. This function is registered as a callback with MySQL. */
static
void
innodb_zdr_dump_range_lsn_recs_update(
    THD*                thd,        /*!< in: thread handle */
    struct st_mysql_sys_var*    var,        /*!< in: pointer to
                            system variable */
    void*                var_ptr,    /*!< out: where the
                            formal string goes */
    const void*            save)        /*!< in: immediate result
                            from check function */
{
   const char*   cmd_format=NULL;
   char  *splitch=NULL,*endptr=NULL,*bgnch=NULL;
   char  dump_format[64*3]={0};
   ulint start_lsn,end_lsn;
   cmd_format = *static_cast<const char*const*>(save);
   if(cmd_format)
   {
     uint formatlen=strlen(cmd_format);
     memcpy(dump_format,cmd_format, formatlen);
     bgnch=dump_format;
     if((splitch=strchr(bgnch,',')))
     {
       *splitch=0;
       start_lsn=strtoull(bgnch,&endptr,10);
       bgnch=splitch+1;
     }
     end_lsn=strtoull(bgnch,&endptr,10);
     FILE *fp_dmp_redo_logblock=0;
     if(!(fp_dmp_redo_logblock=create_dump_file_lsn("dump_range_lsn_recs",start_lsn,end_lsn)))
     {
       sql_print_error("create_dump_file error");
       return;
     }
     time_t         the_time;
     struct tm      *tm_ptr=NULL;
     struct timeval  tv;
     time(&the_time);
     tm_ptr=localtime(&the_time);
     gettimeofday(&tv, NULL);
     fprintf(fp_dmp_redo_logblock,"**** %04d-%02d-%02d %02d:%02d:%02d.%03ld \n",
             tm_ptr->tm_year+1900,tm_ptr->tm_mon+1,tm_ptr->tm_mday,tm_ptr->tm_hour,tm_ptr->tm_min,tm_ptr->tm_sec,tv.tv_usec);
     btr_cur_dump_rangelsn_recs(fp_dmp_redo_logblock,start_lsn,end_lsn);
     fclose(fp_dmp_redo_logblock);
   }
   *static_cast<const char**>(var_ptr)=cmd_format;
   return;
}
/****************************************************************//**
Update the system variable innodb_zdr_dump_doublewrite using the "saved"
value. This function is registered as a callback with MySQL. */
static
void
innodb_zdr_dump_doublewrite(
    THD*                thd,    /*!< in: thread handle */
          struct st_mysql_sys_var*    var,    /*!< in: pointer to system
                              variable */
          void*                save,    /*!< out: immediate result
                              for update function */
          const void*        value)    /*!< in: incoming string */
{
    FILE *fp_dmp_doublewrite=0;
    if(!(fp_dmp_doublewrite=create_dump_file("dump_doublewrite",0,5)))
    {
      sql_print_error("create_dump_file error");
      return;
    }
    time_t         the_time;
    struct tm      *tm_ptr=NULL;
    struct timeval  tv;
    time(&the_time);
    tm_ptr=localtime(&the_time);
    gettimeofday(&tv, NULL);
    fprintf(fp_dmp_doublewrite,"**** %04d-%02d-%02d %02d:%02d:%02d.%03ld \n",
            tm_ptr->tm_year+1900,tm_ptr->tm_mon+1,tm_ptr->tm_mday,tm_ptr->tm_hour,tm_ptr->tm_min,tm_ptr->tm_sec,tv.tv_usec);
    srv_zdr_dump_doublewrite=1;
    btr_cur_dump_doublewrite(0,TRX_SYS_PAGE_NO,fp_dmp_doublewrite);
    srv_zdr_dump_doublewrite=0;
    fclose(fp_dmp_doublewrite);
    return;
}

static MYSQL_SYSVAR_STR(zdr_dump_btreenode, srv_zdr_dump_page,
  PLUGIN_VAR_OPCMDARG,
  "This option is add to dump that page which option value described."
  "set innodb_zdr_dump_page = space_id,pageno,index_id ", 
  innodb_zdr_dump_indexnode_validate, innodb_zdr_dump_indexnode_update, NULL);

static MYSQL_SYSVAR_ULONG(zdr_dump_indextree, srv_zdr_dump_indextree,
  PLUGIN_VAR_OPCMDARG,
  "This option is add to dump that index which option value described."
  "set innodb_zdr_dump_index =index_id ", 
  NULL, innodb_zdr_dump_indextree_update,1, 1,~0UL,0);
  
static MYSQL_SYSVAR_ULONG(zdr_dump_trxlock, srv_zdr_dump_trxlock,
  PLUGIN_VAR_OPCMDARG,
  "This option is add to dump that index which option value described."
  "set innodb_zdr_dump_trxlock =trx_id ", 
  NULL, innodb_zdr_dump_trxlock_update,1, 1,~0UL,0);
  
static MYSQL_SYSVAR_ULONG(zdr_dump_redo_hdr, srv_zdr_dump_redo_hdr,
  PLUGIN_VAR_OPCMDARG,
  "This option is add to dump that index which option value described."
  "set innodb_zdr_dump_redo_hdr =1 ", 
  NULL, innodb_zdr_dump_redo_hdr_update,1, 1,~0UL,0);
  
static MYSQL_SYSVAR_STR(zdr_dump_redo_block, srv_zdr_dump_redo_block,
  PLUGIN_VAR_OPCMDARG,
  "This option is add to dump that page which option value described."
  "set innodb_zdr_dump_redo_block = start_lsn,end_lsn ",
  innodb_zdr_dump_indexnode_validate, innodb_zdr_dump_redo_logblock_update, NULL);
  
static MYSQL_SYSVAR_STR(zdr_dump_range_lsn_recs, srv_zdr_dump_range_lsn_recs,
  PLUGIN_VAR_OPCMDARG,
  "This option is add to dump that page which option value described."
  "set innodb_zdr_dump_range_lsn_recs = start_lsn,end_lsn ",
  innodb_zdr_dump_indexnode_validate, innodb_zdr_dump_range_lsn_recs_update, NULL);
  
static MYSQL_SYSVAR_ULONG(zdr_dump_fsp_hdr, srv_zdr_dump_fsp_hdr,
  PLUGIN_VAR_OPCMDARG,
  "This option is add to dump that index which option value described."
  "set innodb_zdr_dump_fsp_hdr =space_id ", 
  NULL, innodb_zdr_dump_fsp_hdr_update,0, 0,~0UL,0); 
  
static MYSQL_SYSVAR_STR(zdr_dump_fseg_inode, srv_zdr_dump_fseg_inode,
  PLUGIN_VAR_OPCMDARG,
  "This option is add to dump that index which option value described."
  "set innodb_zdr_dump_fseg_inode =space_id,pageno ", 
  innodb_zdr_dump_indexnode_validate, innodb_zdr_dump_fseg_inode_update,NULL); 
  
static MYSQL_SYSVAR_STR(zdr_dump_rseg_node, srv_zdr_dump_rseg_node,
  PLUGIN_VAR_OPCMDARG,
  "This option is add to dump that page which option value described."
  "set innodb_zdr_dump_rseg_node = space_id,pageno ",
  innodb_zdr_dump_indexnode_validate, innodb_zdr_dump_rseg_node_update, NULL);
  
static MYSQL_SYSVAR_STR(zdr_dump_undolog_node, srv_zdr_dump_undolog_node,
  PLUGIN_VAR_OPCMDARG,
  "This option is add to dump that page which option value described."
  "set innodb_zdr_dump_undolog_node = space_id,pageno ",
  innodb_zdr_dump_indexnode_validate, innodb_zdr_dump_undolog_node_update, NULL);
  
static MYSQL_SYSVAR_BOOL(zdr_dump_undo_architecture, srv_zdr_dump_undo_architecture,
  PLUGIN_VAR_OPCMDARG,
  "This option is add to dump that page which option value described."
  "set innodb_zdr_dump_undo_architecture = TRUE ",
  NULL, innodb_zdr_dump_undo_architecture, TRUE); 
  
static MYSQL_SYSVAR_BOOL(zdr_dump_doublewrite, srv_zdr_dump_doublewrite,
  PLUGIN_VAR_OPCMDARG,
  "This option is add to dump that page which option value described."
  "set innodb_zdr_dump_doublewrite = TRUE ",
  NULL, innodb_zdr_dump_doublewrite, FALSE); 