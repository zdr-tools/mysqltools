class Global_THD_manager
{
public:
  THD* get_thd_by_idx(uint i);
  mysql_mutex_t* get_LOCK_thd_list(){return &LOCK_thd_list;}
};