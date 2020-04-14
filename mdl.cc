const char* get_mdl_duraion_name(enum_mdl_duration idx)
{
  switch(idx)
  {
    case MDL_STATEMENT: return "MDL_STATEMENT";
    case MDL_TRANSACTION: return "MDL_TRANSACTION";
    case MDL_EXPLICIT:  return "MDL_EXPLICIT";
    default : return "UNKNOW";
  }
  return "";
}
const char*  get_mdl_type_name(enum_mdl_type mdltype)
{
  switch(mdltype)
  {
   case MDL_INTENTION_EXCLUSIVE : return "MDL_INTENTION_EXCLUSIVE";
   case MDL_SHARED: return "MDL_SHARED";
   case MDL_SHARED_HIGH_PRIO: return "MDL_SHARED_HIGH_PRIO";
   case MDL_SHARED_READ: return "MDL_SHARED_READ";
   case MDL_SHARED_WRITE: return "MDL_SHARED_WRITE";
   case MDL_SHARED_WRITE_LOW_PRIO: return "MDL_SHARED_WRITE_LOW_PRIO";
   case MDL_SHARED_UPGRADABLE: return "MDL_SHARED_UPGRADABLE";
   case MDL_SHARED_READ_ONLY: return "MDL_SHARED_READ_ONLY";
   case MDL_SHARED_NO_WRITE: return "MDL_SHARED_NO_WRITE";
   case MDL_SHARED_NO_READ_WRITE: return "MDL_SHARED_NO_READ_WRITE";
   case MDL_EXCLUSIVE: return "MDL_EXCLUSIVE";
   default: return "UNKNOW";
  }
  return "UNKNOW";
  
}
const char* get_mdl_namespace_name(int nmspace)
{
  switch(nmspace)
  {
     case 0: return "GLOBAL";
     case 1: return "TABLESPACE";
     case 2:return "SCHEMA";
     case 3:return "TABLE";
     case 4:return "FUNCTION";
     case 5:return "PROCEDURE";
     case 6:return "TRIGGER";
     case 7:return "EVENT";
     case 8:return "COMMIT";
     case 9:return "USER_LEVEL_LOCK";
     case 10:return "LOCKING_SERVICE";
     default : return "UNKNOW";
  }
  return "UNKNOW";
}
void MDL_context::dump_mdl_ctx(FILE* fp_dump_mdl)
{
    MDL_ticket *ticket;
    for (int i=0; i < MDL_DURATION_END; i++)
    {
      enum_mdl_duration duration= static_cast<enum_mdl_duration>(i);
      fprintf(fp_dump_mdl,"m_tickets[%s]:\n",get_mdl_duraion_name(duration));
      Ticket_iterator it(m_tickets[duration]);
      int j=0;
      while ((ticket= it++))
      {
         fprintf(fp_dump_mdl,"   %d th granted ticket:\n"
                             "        mdl_type:%s\n"
                             "        key.name_space:%s  key.db_name:%s key.table_name:%s \n"
                             ,j++
                             ,get_mdl_type_name(ticket->get_type())
                             ,get_mdl_namespace_name(ticket->get_key()->mdl_namespace())
                             ,ticket->get_key()->db_name()
                             ,ticket->get_key()->name()
                             );
      }
      Ticket_iterator it1(m_tickets[duration]);
      while ((ticket= it1++))
      {
         MDL_lock* lock=ticket->get_lock();
         if(!lock->m_waiting.is_empty())
         {
            MDL_lock::Ticket_iterator itwait(lock->m_waiting);
            MDL_ticket *ticket_wait;
            int k=0;
            while ((ticket_wait= itwait++))
            {
               if(ticket_wait->get_ctx() != ticket->get_ctx())
               {
                 fprintf(fp_dump_mdl,"   %d th waiting ticket,processlist_id:%u\n"
                                     "        request.mdl_type:%s "
#ifndef DBUG_OFF
"        request.mdl_duration:%s\n"
#else
"\n"
#endif
                                     "        request.key.name_space:%s  request.key.db_name:%s request.key.table_name:%s \n"
                                       ,k++
                                       ,ticket_wait->get_ctx()->get_thd()->thread_id()
                                       ,get_mdl_type_name(ticket_wait->get_type())
#ifndef DBUG_OFF
,get_mdl_duraion_name(ticket_wait->get_duration())
#endif
                                       ,get_mdl_namespace_name(ticket_wait->get_key()->mdl_namespace())
                                       ,ticket_wait->get_key()->db_name()
                                       ,ticket_wait->get_key()->name()
                                       );

               }
            }  
         }
      }
    }
    return;
}