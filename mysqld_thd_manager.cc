THD* Global_THD_manager::get_thd_by_idx(uint i)
{
  THD_array::const_iterator it=thd_list.begin();
  THD* ret= NULL;
  if(i>global_thd_count) return NULL;
  it +=i;
  if(it<thd_list.end()) ret=*it;
  return ret;
}