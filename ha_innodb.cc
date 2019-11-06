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
   tm_ptr=gmtime(&the_time);
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
     tm_ptr=gmtime(&the_time);
     gettimeofday(&tv, NULL);
     fprintf(fp_dmp_page,"**** %04d-%02d-%02d %02d:%02d:%02d.%03ld \n",
             tm_ptr->tm_year+1900,tm_ptr->tm_mon+1,tm_ptr->tm_mday,tm_ptr->tm_hour,tm_ptr->tm_min,tm_ptr->tm_sec,tv.tv_usec);
     btr_cur_dump_index_record(space_id,page_no,fp_dmp_page);
     fclose(fp_dmp_page);
   }
   *static_cast<const char**>(var_ptr)=cmd_format;   
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
  
static struct st_mysql_sys_var* innobase_system_variables[]= {
/*文件中已经存在的变量*/
  MYSQL_SYSVAR(zdr_dump_indextree),
  MYSQL_SYSVAR(zdr_dump_btreenode)
}
