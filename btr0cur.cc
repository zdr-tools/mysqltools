/*****************************************************************//**
Dump file header*/
static void 
btr_cur_dump_file_header(FILE* fp_page,const page_t* page)
{
  const byte*    rec=page;
  fprintf(fp_page,"--------------------------------\n"
  "FILE_HEADER:\n");
  fprintf(fp_page,"FIL_PAGE_SPACE_OR_CHKSUM: %02x %02x %02x %02x\n",rec[0],rec[1],rec[2],rec[3]);
  rec+=4;
  fprintf(fp_page,"FIL_PAGE_OFFSET: %02x %02x %02x %02x\n",rec[0],rec[1],rec[2],rec[3]);
  rec+=4;
  fprintf(fp_page,"FIL_PAGE_PREV: %02x %02x %02x %02x\n",rec[0],rec[1],rec[2],rec[3]);
  rec+=4;
  fprintf(fp_page,"FIL_PAGE_NEXT: %02x %02x %02x %02x\n",rec[0],rec[1],rec[2],rec[3]);
  rec+=4;
  fprintf(fp_page,"FIL_PAGE_LSN: %02x %02x %02x %02x %02x %02x %02x %02x\n",rec[0],rec[1],rec[2],rec[3],rec[4],rec[5],rec[6],rec[7]);
  rec+=8;
  fprintf(fp_page,"FIL_PAGE_TYPE: %02x %02x\n",rec[0],rec[1]);
  rec+=2;
  fprintf(fp_page,"FIL_PAGE_FILE_FLUSH_LSN: %02x %02x %02x %02x %02x %02x %02x %02x\n",rec[0],rec[1],rec[2],rec[3],rec[4],rec[5],rec[6],rec[7]);
  rec+=8;
  fprintf(fp_page,"FIL_PAGE_SPACE_ID: %02x %02x %02x %02x\n",rec[0],rec[1],rec[2],rec[3]);
}
/*****************************************************************//**
Dump page header*/
static void 
btr_cur_dump_page_header(FILE* fp_page,const page_t* page)
{
  const byte*    rec=page;
  fprintf(fp_page,"--------------------------------\n"
  "PAGE_HEADER :\n");
  fprintf(fp_page,"PAGE_N_DIR_SLOTS: %02x %02x (%lu)\n",rec[0],rec[1],mach_read_from_2(rec));
  rec+=2;
  fprintf(fp_page,"PAGE_HEAP_TOP: %02x %02x (%lu)\n",rec[0],rec[1],mach_read_from_2(rec));
  rec+=2;
  fprintf(fp_page,"PAGE_N_HEAP: %02x %02x (%lu)\n",rec[0],rec[1],mach_read_from_2(rec));
  rec+=2;
  fprintf(fp_page,"PAGE_FREE: %02x %02x (%lu)\n",rec[0],rec[1],mach_read_from_2(rec));
  rec+=2;
  fprintf(fp_page,"PAGE_GARBAGE: %02x %02x (%lu)\n",rec[0],rec[1],mach_read_from_2(rec));
  rec+=2;
  fprintf(fp_page,"PAGE_LAST_INSERT: %02x %02x (%lu)\n",rec[0],rec[1],mach_read_from_2(rec));
  rec+=2;
  fprintf(fp_page,"PAGE_DIRECTION: %02x %02x (%lu)\n",rec[0],rec[1],mach_read_from_2(rec));
  rec+=2;
  fprintf(fp_page,"PAGE_N_DIRECTION: %02x %02x (%lu)\n",rec[0],rec[1],mach_read_from_2(rec));
  rec+=2;
  fprintf(fp_page,"PAGE_N_RECS: %02x %02x (%lu)\n",rec[0],rec[1],mach_read_from_2(rec));
  rec+=2;
  fprintf(fp_page,"PAGE_MAX_TRX_ID: %02x %02x %02x %02x %02x %02x %02x %02x (%lu)\n",rec[0],rec[1],rec[2],rec[3],rec[4],rec[5],rec[6],rec[7],mach_read_from_8(rec));
  rec+=8;
  fprintf(fp_page,"PAGE_LEVEL: %02x %02x (%lu)\n",rec[0],rec[1],mach_read_from_2(rec));
  rec+=2;
  fprintf(fp_page,"PAGE_INDEX_ID: %02x %02x %02x %02x %02x %02x %02x %02x  (%lu)\n",rec[0],rec[1],rec[2],rec[3],rec[4],rec[5],rec[6],rec[7],mach_read_from_8(rec));
  rec+=8;
  fprintf(fp_page,"PAGE_BTR_SEG_LEAF: \n"
   "   SPACE_ID: %02x %02x %02x %02x \n"
   "   PAGE_NO:  %02x %02x %02x %02x (%lu)\n"
   "   OFFSET:   %02x %02x (%lu)\n",rec[0],rec[1],rec[2],rec[3]
                           ,rec[4],rec[5],rec[6],rec[7],mach_read_from_4(rec+4)
                           ,rec[8],rec[9],mach_read_from_2(rec+8));
  rec+=10;
  fprintf(fp_page,"PAGE_BTR_SEG_TOP: \n"
   "   SPACE_ID: %02x %02x %02x %02x \n"
   "   PAGE_NO:  %02x %02x %02x %02x (%lu)\n"
   "   OFFSET:   %02x %02x (%lu)\n",rec[0],rec[1],rec[2],rec[3]
                           ,rec[4],rec[5],rec[6],rec[7],mach_read_from_4(rec+4)
                           ,rec[8],rec[9],mach_read_from_2(rec+8));
}
/*****************************************************************//**
Dump index page slots*/
static void
btr_cur_dump_page_slots(
FILE* fp_page,
const page_t* page)
{
    ulint            n;
    ulint            i;
    page_dir_slot_t*    slot;

    n = page_dir_get_n_slots(page);

    fprintf(fp_page, "--------------------------------\n"
        "PAGE SLOTS\n"
        "Page address %p\n"
        "Directory stack top at offs: %lu; number of slots: %lu\n",
        page, (ulong) page_offset(page_dir_get_nth_slot(page, n - 1)),
        (ulong) n);
    for (i = 0; i < n; i++) {
        slot = page_dir_get_nth_slot(page, i);
            fprintf(fp_page,
                "Contents of slot: %lu: addr:%p n_owned: %lu,"
                " rec offs: %lu\n",
                (ulong) i,slot,
                (ulong) page_dir_slot_get_n_owned(slot),
                (ulong)
                page_offset(page_dir_slot_get_rec(slot)));
    }
    fprintf(fp_page, "Total of %lu records\n"
        "--------------------------------\n",
        (ulong) (PAGE_HEAP_NO_USER_LOW + page_get_n_recs(page)));

}
/*****************************************************************//**
Dump record extra_size*/
static void
btr_cur_dump_record_extra_size(
     FILE*               fp_page, /*!< in: file pointer*/
     const rec_t*        rec,    /*!< in: physical record */
     const dict_index_t* index  /*!< in: record descriptor */
)
{
    ulint    n=0;

    ut_ad(rec);
    ut_ad(index);
    if (dict_table_is_comp(index->table)) 
    {
        switch (UNIV_EXPECT(rec_get_status(rec),
                    REC_STATUS_ORDINARY)) 
        {
        case REC_STATUS_ORDINARY:
            n = dict_index_get_n_fields(index);
            break;
        case REC_STATUS_NODE_PTR:
            /* Node pointer records consist of the
            uniquely identifying fields of the record
            followed by a child page number field. */
            n = dict_index_get_n_unique_in_tree_nonleaf(index) + 1;
            break;
        case REC_STATUS_INFIMUM:
        case REC_STATUS_SUPREMUM:
            /* infimum or supremum record */
            n = 1;
            break;
        default:
            ut_error;
            return ;
        }
    } 
    else 
    {
        n = rec_get_n_fields_old(rec);
    }
    ulint    i    = 0;    
    if (dict_table_is_comp(index->table))
    {
        const byte*    nulls;
        const byte*    lens;
        const byte*    lens_end;
        dict_field_t*    field;
        ulint        null_mask;
        ulint        vfield_cnt=0;
        ulint        vfield_byte=0;
        ulint        null_fields=0;
        ulint        status = rec_get_status(rec);
        ulint        n_node_ptr_field = ULINT_UNDEFINED;

        switch (UNIV_EXPECT(status, REC_STATUS_ORDINARY)) {
        case REC_STATUS_INFIMUM:
        case REC_STATUS_SUPREMUM:
            /* the field is 8 bytes long */
          fprintf(fp_page,"rec_new_extra_5_bytes: %02x %02x %02x %02x %02x\n",rec[-5],rec[-4],rec[-3],rec[-2],rec[-1]);
          return;
        case REC_STATUS_NODE_PTR:
            n_node_ptr_field
                = dict_index_get_n_unique_in_tree_nonleaf(
                    index);
            break;
        case REC_STATUS_ORDINARY:
            break;
        }

        nulls = rec - (REC_N_NEW_EXTRA_BYTES + 1);
        lens =lens_end= nulls - UT_BITS_IN_BYTES(index->n_nullable);
        null_mask = 1;

        /* read the lengths of fields 0..n */
        do 
        {
            ulint    len=0;
            if (UNIV_UNLIKELY(i == n_node_ptr_field)) {
                len = REC_NODE_PTR_SIZE;
                goto resolved;
            }

            field = dict_index_get_nth_field(index, i);
            if (!(dict_field_get_col(field)->prtype & DATA_NOT_NULL)) 
            {
                /* nullable field => read the null flag */

                if (UNIV_UNLIKELY(!(byte) null_mask)) 
                {
                    nulls--;
                    null_mask = 1;
                }

                if (*nulls & null_mask) 
                {
                    null_mask <<= 1;
                    /* No length is stored for NULL fields.
                    We do not advance offs, and we set
                    the length to zero and enable the
                    SQL NULL flag in offsets[]. */
                    len = 0;
                    null_fields++;
                    goto resolved;
                }
                null_mask <<= 1;
            }

            if (UNIV_UNLIKELY(!field->fixed_len)) 
            {
                const dict_col_t*    col
                    = dict_field_get_col(field);
                /* DATA_POINT should always be a fixed
                length column. */
                /* Variable-length field: read the length */
                len = *lens--;
                vfield_cnt++;
                vfield_byte++;
                /* If the maximum length of the field
                is up to 255 bytes, the actual length
                is always stored in one byte. If the
                maximum length is more than 255 bytes,
                the actual length is stored in one
                byte for 0..127.  The length will be
                encoded in two bytes when it is 128 or
                more, or when the field is stored
                externally. */
                if (DATA_BIG_COL(col)) 
                {
                    if (len & 0x80) 
                    {
                        lens--;
                        vfield_byte++;
                        goto resolved;
                    }
                }

            } 
            else 
            {
                len = field->fixed_len;
            }
resolved:
         len=0;
        } while (++i < n);
        fprintf(fp_page,"(%lu) variable-length fields  (%lu Bytes):\n",vfield_cnt,vfield_byte);
        uint i=0;
        lens++;
        for(i=0;lens<=lens_end;)
        {  
           i++;
           fprintf(fp_page," %02x",*lens++);
           if(!(i%16))
            fprintf(fp_page,"\n");
        }
        if(i!=0)
           fprintf(fp_page,"\n");
        fprintf(fp_page,"(%u) nullable fields and %lu fields is null. (%u Bytes): \n",(index->n_nullable),null_fields
                                                                                ,UT_BITS_IN_BYTES(index->n_nullable));
        for(i=0;lens<=nulls;lens++)
        {  
           i++;
           fprintf(fp_page," %u%u%u%u%u%u%u%u ",(1&(lens[0]>>7)),(1&(lens[0]>>6))
                                               ,(1&(lens[0]>>5)),(1&(lens[0]>>4))
                                               ,(1&(lens[0]>>3)),(1&(lens[0]>>2)),
                                               (1&(lens[0]>>1)),(1&lens[0])
                                );
           if(!(i%2))
            fprintf(fp_page,"\n");
        }
        if(i!=0)
        fprintf(fp_page,"\n");
        fprintf(fp_page,"rec_new_extra_5_bytes: %02x %02x %02x %02x %02x\n",rec[-5],rec[-4],rec[-3],rec[-2],rec[-1]);
    }
    else
    {
        /* Old-style record: determine extra size and end offsets */
        if(page_rec_is_infimum(rec))
        {
           fprintf(fp_page,"rec_old_extra_6_bytes: %02x %02x %02x %02x %02x %02x\n",rec[-6],rec[-5],rec[-4],rec[-3],rec[-2],rec[-1]);
           return;
        }
        if(page_rec_is_supremum(rec))
        {
          fprintf(fp_page,"rec_old_extra_6_bytes: %02x %02x %02x %02x %02x %02x\n",rec[-6],rec[-5],rec[-4],rec[-3],rec[-2],rec[-1]);
          return;
        }
        if (rec_get_1byte_offs_flag(rec)) 
        {
            /* Determine offsets to fields */
            fprintf(fp_page,"data offsets(one-byte):\n");
            do {
            fprintf(fp_page," %02x",rec[-(REC_N_OLD_EXTRA_BYTES + n -i)]);
            i++;
            if(0==i%16)
              fprintf(fp_page,"\n");
            } while (i<n);
            if(i!=0)
              fprintf(fp_page,"\n");
        } 
        else 
        {
            /* Determine offsets to fields */
            fprintf(fp_page,"data offsets(two-bytes):\n");
            do {
                fprintf(fp_page," %02x %02x",rec[-(REC_N_OLD_EXTRA_BYTES + 2*(n -i))]
                                            ,rec[-(REC_N_OLD_EXTRA_BYTES + 2*(n -i)-1)]);
                i++;
                if(0==i%8)
                  fprintf(fp_page,"\n");
            } while (++i < n);
            if(i!=0)
              fprintf(fp_page,"\n");
        }
        fprintf(fp_page,"rec_old_extra_6_bytes: %02x %02x %02x %02x %02x %02x\n",rec[-6],rec[-5],rec[-4],rec[-3],rec[-2],rec[-1]);
    }
    return;
}
/*****************************************************************//**
Dump index one record*/
static void
btr_cur_dump_one_record(
        FILE*               fp_page, /*!< in: file pointer*/
        const rec_t*        rec,    /*!< in: physical record */
        const dict_index_t* index  /*!< in: record descriptor */
        ) 
{
    ulint    n;
    ut_ad(rec);
    ut_ad(index);
    if (dict_table_is_comp(index->table)) 
    {
        switch (UNIV_EXPECT(rec_get_status(rec),
                    REC_STATUS_ORDINARY)) 
        {
        case REC_STATUS_ORDINARY:
            n = dict_index_get_n_fields(index);
            break;
        case REC_STATUS_NODE_PTR:
            /* Node pointer records consist of the
            uniquely identifying fields of the record
            followed by a child page number field. */
            n = dict_index_get_n_unique_in_tree_nonleaf(index) + 1;
            break;
        case REC_STATUS_INFIMUM:
        case REC_STATUS_SUPREMUM:
            /* infimum or supremum record */
            n = 1;
            break;
        default:
            return ;
        }
    } 
    else 
    {
        n = rec_get_n_fields_old(rec);
    }
    /* The offsets header consists of the allocation size at
    offsets[0] and the REC_OFFS_HEADER_SIZE bytes. */
    ulint    i    = 0;    
    if (dict_table_is_comp(index->table))
    {
        const byte*    nulls;
        const byte*    lens;
        const byte*    rec_ptr=rec;
        dict_field_t*    field;
        ulint        null_mask;
        ulint        status = rec_get_status(rec);
        ulint        n_node_ptr_field = ULINT_UNDEFINED;

        switch (UNIV_EXPECT(status, REC_STATUS_ORDINARY)) {
        case REC_STATUS_INFIMUM:
            fprintf(fp_page," This record is infimum.\n");
            return;
        case REC_STATUS_SUPREMUM:
            /* the field is 8 bytes long */
            fprintf(fp_page," This record is supremum.\n");
            return;
        case REC_STATUS_NODE_PTR:
            n_node_ptr_field
                = dict_index_get_n_unique_in_tree_nonleaf(
                    index);
            break;
        case REC_STATUS_ORDINARY:
            break;
        }

        nulls = rec - (REC_N_NEW_EXTRA_BYTES + 1);
        lens = nulls - UT_BITS_IN_BYTES(index->n_nullable);
        null_mask = 1;

        /* read the lengths of fields 0..n */
        do {
            ulint    len=0;
            if (UNIV_UNLIKELY(i == n_node_ptr_field)) {
                len = REC_NODE_PTR_SIZE;
                goto resolved;
            }

            field = dict_index_get_nth_field(index, i);
            if (!(dict_field_get_col(field)->prtype & DATA_NOT_NULL)) 
            {
                /* nullable field => read the null flag */

                if (UNIV_UNLIKELY(!(byte) null_mask)) 
                {
                    nulls--;
                    null_mask = 1;
                }

                if (*nulls & null_mask) 
                {
                    null_mask <<= 1;
                    /* No length is stored for NULL fields.
                    We do not advance offs, and we set
                    the length to zero and enable the
                    SQL NULL flag in offsets[]. */
                    len = 0;
                    goto resolved;
                }
                null_mask <<= 1;
            }

            if (UNIV_UNLIKELY(!field->fixed_len)) 
            {
                const dict_col_t*    col
                    = dict_field_get_col(field);
                /* DATA_POINT should always be a fixed
                length column. */
                /* Variable-length field: read the length */
                len = *lens--;
                /* If the maximum length of the field
                is up to 255 bytes, the actual length
                is always stored in one byte. If the
                maximum length is more than 255 bytes,
                the actual length is stored in one
                byte for 0..127.  The length will be
                encoded in two bytes when it is 128 or
                more, or when the field is stored
                externally. */
                if (DATA_BIG_COL(col)) 
                {
                    if (len & 0x80) 
                    {
                        /* 1exxxxxxx xxxxxxxx */

                        len <<= 8;
                        len |= *lens--;

                        /* B-tree node pointers
                        must not contain externally
                        stored columns.  Thus
                        the "e" flag must be 0. */
                        ut_a(!(len & 0x4000));
                        len = len & 0x3fff;

                        goto resolved;
                    }
                }

            } 
            else 
            {
                len = field->fixed_len;
            }
resolved:
        if(UNIV_UNLIKELY(i == n_node_ptr_field)) 
        {
            fprintf(fp_page,"col %ld: nm: [child_page_no] len: %ld \n",i,len);
        }
        else
        {
            fprintf(fp_page,"col %ld: nm:[%s] len: %ld \n",i,(const char *)(field->name),len);
        }
        if(0==memcmp("DB_ROLL_PTR",(const char *)(field->name),11) && 
           11==strlen((const char *)(field->name))
          )
        {
          roll_ptr_t  roll_ptr=trx_read_roll_ptr(rec_ptr);
          ibool  is_insert;
          ulint  rseg_id;
          ulint  page_no;
          ulint  page_offset;
          trx_undo_decode_roll_ptr(roll_ptr,&is_insert,&rseg_id,&page_no,&page_offset);
          fprintf(fp_page,"rseg_id:%lu insert:%lu space_id:%lu page_no:%lu offset:%lu \n",
                         rseg_id,is_insert,trx_undo_get_space_id(rseg_id),page_no,page_offset);
        }
        for(ulint j=0;j<len;)
        {
          fprintf(fp_page," %02x",rec_ptr[j++]);
          if(!(j%25))
          fprintf(fp_page,"\n");
        }
        rec_ptr+=len;
        if(len)
        fprintf(fp_page,"\n");
      } while (++i < n);
    }
    else
    {
        /* Old-style record: determine extra size and end offsets */
        ulint  offs_prev=0;
        ulint  offs_new=0;
        ulint  len=0;
        const byte*    rec_ptr=rec;
        dict_field_t*    field;
        if(page_rec_is_infimum(rec))
        {
          fprintf(fp_page,"this record is infimum(Old-style).\n");
          return;
        }
        if(page_rec_is_supremum(rec))
        {
          fprintf(fp_page,"this record is supremum.\n");
          return;
        }
        if (rec_get_1byte_offs_flag(rec)) 
        {
            /* Determine offsets to fields */
            do {
                offs_new= rec_1_get_field_end_info(rec, i);
                if (offs_new & REC_1BYTE_SQL_NULL_MASK) 
                {
                    offs_new &= ~REC_1BYTE_SQL_NULL_MASK;
                }
                len=(offs_new>offs_prev)?(offs_new-offs_prev):0;
                offs_prev=offs_new;
                field=dict_index_get_nth_field(index, i);
                fprintf(fp_page,"col %ld: nm: [%s] len: %ld \n",i,(const char *)(field->name),len);
                if(0==memcmp("DB_ROLL_PTR",(const char *)(field->name),11) && 
                   11==strlen((const char *)(field->name))
                  )
                {
                  roll_ptr_t  roll_ptr=trx_read_roll_ptr(rec_ptr);
                  ibool  is_insert;
                  ulint  rseg_id;
                  ulint  page_no;
                  ulint  page_offset;
                  trx_undo_decode_roll_ptr(roll_ptr,&is_insert,&rseg_id,&page_no,&page_offset);
                  fprintf(fp_page,"rseg_id:%lu insert:%lu space_id:%lu page_no:%lu offset:%lu \n",
                         rseg_id,is_insert,trx_undo_get_space_id(rseg_id),page_no,page_offset);
                }
                for(ulint j=0;j<len;)
                {
                  fprintf(fp_page," %02x",rec_ptr[j++]);
                 if(!(j%25))
                  fprintf(fp_page,"\n");
                }
                rec_ptr+=len;
                if(len)
                fprintf(fp_page,"\n");
            } while (++i < n);
        } 
        else 
        {
            /* Determine offsets to fields */
            do 
            {
                offs_new = rec_2_get_field_end_info(rec, i);
                if (offs_new & REC_2BYTE_SQL_NULL_MASK) 
                {
                    offs_new &= ~REC_2BYTE_SQL_NULL_MASK;
                }
                if (offs_new & REC_2BYTE_EXTERN_MASK) 
                {
                    offs_new &= ~REC_2BYTE_EXTERN_MASK;
                }
                len=(offs_new>offs_prev)?(offs_new-offs_prev):0;
                offs_prev=offs_new;
                field=dict_index_get_nth_field(index, i);
                fprintf(fp_page,"col %ld: nm: [%s] len: %ld \n",i,(const char *)(field->name),len);
                if(0==memcmp("DB_ROLL_PTR",(const char *)(field->name),11) && 
                   11==strlen((const char *)(field->name))
                  )
                {
                  roll_ptr_t  roll_ptr=trx_read_roll_ptr(rec_ptr);
                  ibool  is_insert;
                  ulint  rseg_id;
                  ulint  page_no;
                  ulint  page_offset;
                  trx_undo_decode_roll_ptr(roll_ptr,&is_insert,&rseg_id,&page_no,&page_offset);
                  fprintf(fp_page,"rseg_id:%lu insert:%lu space_id:%lu page_no:%lu offset:%lu \n",
                         rseg_id,is_insert,trx_undo_get_space_id(rseg_id),page_no,page_offset);
                }
                
                for(ulint j=0;j<len;)
                {
                  fprintf(fp_page," %02x",rec_ptr[j++]);
                 if(!(j%25))
                  fprintf(fp_page,"\n");
                }
                rec_ptr+=len;
                if(len)
                fprintf(fp_page,"\n");
            } while (++i < n);
        
        }
    }
    return;
}
/*****************************************************************//**
Dump index page record*/
void
btr_cur_dump_index_record(
  ulint         space_id,
  ulint         page_no,
  FILE*         fp_page
)
{
    page_cur_t      page_cursor;
   buf_block_t*    block=NULL;
   page_t*         page;
   mtr_t           mtr;
   rec_t*        node_ptr;
   uint           i=0,n_recs=0;

   page_id_t  page_id(space_id,page_no);
   bool            found;
   const page_size_t&    page_size
       = fil_space_get_page_size(page_id.space(), &found);
   if(false==found)
   {
      fprintf(fp_page,"space_id=%ld is not exist\n",space_id);
      return;
   }
   mtr_start(&mtr);
   block = buf_page_get_gen(page_id, page_size, RW_NO_LATCH, NULL,
                    BUF_GET, __FILE__, __LINE__, &mtr);
   if(NULL==block)
   {
     fprintf(fp_page,"space_id=%ld page_no=%ld is not exist\n",space_id,page_no);
     return;
   }
   page = buf_block_get_frame(block);
   if(FIL_PAGE_INDEX != fil_page_get_type(page) &&
      FIL_PAGE_RTREE != fil_page_get_type(page))
   {
      fprintf(fp_page,"space_id=%ld page_no=%ld File page type is not B-tree node \n",space_id,page_no);
      return;
   }
   index_id_t index_id=btr_page_get_index_id(page);
   dict_index_t* index=dict_index_get_if_in_cache(index_id);
   if(NULL==index)
   {
     fprintf(fp_page,"The index_id %ld not in cache",index_id);
     return;
   }
   page_cursor.index=index;
   page_cur_set_before_first(block, &page_cursor);
   ibool  is_comp=dict_table_is_comp(index->table);
   fprintf(fp_page,"-----begin dump page record\n");
    btr_cur_dump_file_header(fp_page,page);
    btr_cur_dump_page_header(fp_page,page+PAGE_HEADER);
    btr_cur_dump_page_slots(fp_page,page);
   /*for(; !page_cur_is_after_last(&page_cursor);page_cur_move_to_next(&page_cursor))*/
   n_recs=page_get_n_recs(page);
   node_ptr = page_cur_get_rec(&page_cursor);
   for(;i<=n_recs+1;node_ptr=page_rec_get_next(node_ptr))
   {
      fprintf(fp_page,"row#%d del:%ld n_owned:%ld heap_no:%ld current_offs:%ld next_record=%ld\n",
      i++,rec_get_deleted_flag(node_ptr,is_comp),is_comp? rec_get_n_owned_new(node_ptr):rec_get_n_owned_old(node_ptr),is_comp? rec_get_heap_no_new(node_ptr):rec_get_heap_no_old(node_ptr)
       ,page_offset(node_ptr),rec_get_next_offs(node_ptr,is_comp));
       btr_cur_dump_record_extra_size(fp_page,node_ptr,index);
       btr_cur_dump_one_record(fp_page,node_ptr,index);
    }
    
    node_ptr=page_header_get_ptr(page,PAGE_FREE);
    if(node_ptr)
    {
       fprintf(fp_page,"---------begin purged records dump\n");    
       i=0;
       while(node_ptr)
       {
           fprintf(fp_page,"row#%d del:%ld n_owned:%ld heap_no:%ld current_offs:%lu next_record:%ld\n",
           i++,rec_get_deleted_flag(node_ptr,is_comp),is_comp? rec_get_n_owned_new(node_ptr):rec_get_n_owned_old(node_ptr),is_comp? rec_get_heap_no_new(node_ptr):rec_get_heap_no_old(node_ptr)
            ,page_offset(node_ptr),rec_get_next_offs(node_ptr,is_comp));
            btr_cur_dump_record_extra_size(fp_page,node_ptr,index);
            btr_cur_dump_one_record(fp_page,node_ptr,index);
         node_ptr=page_rec_get_next(node_ptr);
       }
       fprintf(fp_page,"---------end purged records dump\n");
    }
    fprintf(fp_page,"-----end dump page record\n");
   mtr_commit(&mtr);
   return;
}
/*****************************************************************//**
Dump index page addr*/
void
btr_cur_dump_index_page(
   ulint         space_id,
   ulint         page_no,
   btr_cur_t*    cursor,
   page_size_t   page_size,
    uint           roothegiht,
  FILE*            fp_btrcur
)
{
    char  spacemsg[128]={0};
    buf_block_t*    block=NULL;
    page_t*         page;
    mtr_t           mtr;
    page_cur_t      page_cursor;
    uint level      =0;
    rec_t*        node_ptr;
    ulint        offsets_[REC_OFFS_NORMAL_SIZE];
    ulint*        offsets        = offsets_;
    mem_heap_t*    heap        = NULL;
    page_id_t  page_id(space_id,page_no);
    mtr_start(&mtr);
    block = buf_page_get_gen(page_id, page_size, RW_NO_LATCH, NULL,
                     BUF_GET, __FILE__, __LINE__, &mtr);
    page = buf_block_get_frame(block);
    level = btr_page_get_level(page, mtr);
    memset(spacemsg,' ',(roothegiht-level)*5);
    if(level>=1)
    {
       fprintf(fp_btrcur,"%sbranch:[%p][%ld]level:%u \n",spacemsg,page,page_get_page_no(page),level);
       page_cur_set_before_first(block, &page_cursor);
       page_cur_move_to_next(&page_cursor);
       for(; !page_cur_is_after_last(&page_cursor);page_cur_move_to_next(&page_cursor))
       {
          node_ptr = page_cur_get_rec(&page_cursor);
           rec_offs_init(offsets_);
          offsets = rec_get_offsets(node_ptr, cursor->index, offsets,
                      ULINT_UNDEFINED, &heap);
          ulint child_page_no=btr_node_ptr_get_child_page_no(node_ptr, offsets);
          btr_cur_dump_index_page(space_id,child_page_no,cursor,page_size,roothegiht,fp_btrcur);
        }
    }
    else
    {
      fprintf(fp_btrcur,"%sleaf:[%p][%ld]\n",spacemsg,page,page_get_page_no(page));
      mtr_commit(&mtr);
      return;   
    }
    if (heap) 
    {
      mem_heap_free(heap);
    }
    mtr_commit(&mtr);
    return;
}
/*****************************************************************//**
Fetch every node of an index*/
void 
btr_cur_dump_index_prefetch(
   dict_index_t*    index,        /*!< in: index */
   FILE*            fp_btrcur
)
{
    btr_cur_t      pbtrcur;
    mtr_t          mtr;
    ulint        root_height = 0; /* remove warning */
    
    page_id_t        page_id(dict_index_get_space(index),
                    dict_index_get_page(index));
    const page_size_t&    page_size = dict_table_page_size(index->table);
    buf_block_t*    block;        
    page_t*        page;
    mtr_start(&mtr);
    block = buf_page_get_gen(page_id, page_size, RW_NO_LATCH, NULL,
                     BUF_GET, __FILE__, __LINE__, &mtr);
    page = buf_block_get_frame(block);
    root_height = btr_page_get_level(page, mtr);
    mtr_commit(&mtr);
    pbtrcur.index=index;
    fprintf(fp_btrcur,"-----begin index tree dump\n");
    btr_cur_dump_index_page(dict_index_get_space(index),dict_index_get_page(index),&pbtrcur,page_size,root_height,fp_btrcur);
    fprintf(fp_btrcur,"-----end index tree dump\n");
}
/*****************************************************************/
/**dump undo log trx_sys*/
static 
void
btr_cur_dump_undo_mngr_hdr
(
FILE*         fp_dmp_undo_arch,
const         page_t*   page)
{
  const byte*    rec=page;
  fprintf(fp_dmp_undo_arch,"--------------------------------\n");
  fprintf(fp_dmp_undo_arch,"TRX_SYS_TRX_ID_STORE: %02x %02x %02x %02x %02x %02x %02x %02x (%lu)\n"
          ,rec[0],rec[1],rec[2],rec[3],rec[4],rec[5],rec[6],rec[7],mach_read_from_8(rec));
  rec+=8;
  fprintf(fp_dmp_undo_arch,"TRX_SYS_FSEG_HEADER (segment header for the tablespace segment the trx system is created into):\n"
  "  SPACE_ID: %02x %02x %02x %02x (%lu)\n"
  "  PAGE_NO : %02x %02x %02x %02x (%lu)\n"
  "  OFFSET  : %02x %02x (%lu)\n",rec[0],rec[1],rec[2],rec[3],mach_read_from_4(rec)
                                 ,rec[4],rec[5],rec[6],rec[7],mach_read_from_4(rec+4)
                                 ,rec[8],rec[9],mach_read_from_2(rec+8));
  rec+=10;
  fprintf(fp_dmp_undo_arch,"TRX_SYS_RSEGS (the start of the array of rollback segment specification slots):\n");
  for(uint i=0;i<128;rec+=8)
  {
   fprintf(fp_dmp_undo_arch," (%u) slot rseg:\n"
  "  SPACE_ID: %02x %02x %02x %02x (%lu)\n"
  "  PAGE_NO : %02x %02x %02x %02x (%lu)\n",i++,rec[0],rec[1],rec[2],rec[3],mach_read_from_4(rec)
                                            ,rec[4],rec[5],rec[6],rec[7],mach_read_from_4(rec+4)
          );
  }
  return;
}
/*****************************************************************/
/**dump rollback segment hdr*/
static 
void
btr_cur_dump_rseg_hdr
(
FILE*         fp_dmp_rseg_hdr,
const         page_t*   page)
{
    const byte*    rec=page;
    fprintf(fp_dmp_rseg_hdr,"--------------------------------\n");
    fprintf(fp_dmp_rseg_hdr,"TRX_RSEG_MAX_SIZE (maximum allowed size for rollback segment in pages):\n"
     " %02x %02x %02x %02x (%lu)\n"
            ,rec[0],rec[1],rec[2],rec[3],mach_read_from_4(rec));
    rec+=4;
    fprintf(fp_dmp_rseg_hdr,"TRX_RSEG_HISTORY_SIZE (number of file pages occupied by the logs in the history list):\n"
     " %02x %02x %02x %02x (%lu)\n"
            ,rec[0],rec[1],rec[2],rec[3],mach_read_from_4(rec));
    rec+=4;
    fprintf(fp_dmp_rseg_hdr,"TRX_RSEG_HISTORY (The update undo logs for committed transactions):\n"
    "  FLS_LEN: %02x %02x %02x %02x (%lu)\n"
    "  FLS_FIRST:\n"
    "     PAGE_NO: %02x %02x %02x %02x (%lu)\n"
    "     OFFSET  : %02x %02x (%lu)\n"
    "  FLS_LAST:\n"
    "     PAGE_NO: %02x %02x %02x %02x (%lu)\n"
    "     OFFSET  : %02x %02x (%lu)\n",rec[0],rec[1],rec[2],rec[3],mach_read_from_4(rec)
                                   ,rec[4],rec[5],rec[6],rec[7],mach_read_from_4(rec+4)
                                   ,rec[8],rec[9],mach_read_from_2(rec+8)
                                   ,rec[10],rec[11],rec[12],rec[13],mach_read_from_4(rec+10)
                                   ,rec[14],rec[15],mach_read_from_2(rec+14));
    rec+=16;
    fprintf(fp_dmp_rseg_hdr,"TRX_RSEG_FSEG_HEADER (Header for the file segment where this page is placed):\n"
        "  SPACE_ID: %02x %02x %02x %02x (%lu)\n"
        "  PAGE_NO : %02x %02x %02x %02x (%lu)\n"
        "  OFFSET  : %02x %02x (%lu)\n",rec[0],rec[1],rec[2],rec[3],mach_read_from_4(rec)
                                       ,rec[4],rec[5],rec[6],rec[7],mach_read_from_4(rec+4)
                                       ,rec[8],rec[9],mach_read_from_2(rec+8));
    rec+=10;
    fprintf(fp_dmp_rseg_hdr,"TRX_RSEG_UNDO_SLOTS (Undo log segment slots page_no):\n");
    for(uint i=0;i<1024;rec+=4)
    {
     fprintf(fp_dmp_rseg_hdr," (%04u): %02x %02x %02x %02x (%10lu)",i++,rec[0],rec[1],rec[2],rec[3],mach_read_from_4(rec));
     if(!(i%4))
      fprintf(fp_dmp_rseg_hdr,"\n");
    }
    fprintf(fp_dmp_rseg_hdr,"\n");
    return;

}
/*****************************************************************/
/**dump undo page hdr*/
static 
void
btr_cur_dump_undo_page_hdr
(
FILE*         fp_dmp_undo_page_hdr,
const         page_t*   page)
{
    const byte*    rec=page;
    fprintf(fp_dmp_undo_page_hdr,"--------------------------------\n");
    fprintf(fp_dmp_undo_page_hdr,"TRX_UNDO_PAGE_TYPE(1-insert 2-update): %02x %02x \n",rec[0],rec[1]);
    rec+=2;
    fprintf(fp_dmp_undo_page_hdr,"TRX_UNDO_PAGE_START:"
    " %02x %02x (%lu)\n",rec[0],rec[1],mach_read_from_2(rec));
    rec+=2;
    fprintf(fp_dmp_undo_page_hdr,"TRX_UNDO_PAGE_FREE:"
    " %02x %02x (%lu)\n",rec[0],rec[1],mach_read_from_2(rec));
    rec+=2;
    fprintf(fp_dmp_undo_page_hdr,"TRX_UNDO_PAGE_NODE(The file list node in the chain of undo log pages):\n"
    " FIL_PREV:\n"
    "   PAGE_NO: %02x %02x %02x %02x (%lu)\n"
    "   OFFSET : %02x %02x (%lu)\n"
    " FIL_NEXT:\n"
    "   PAGE_NO: %02x %02x %02x %02x (%lu)\n"
    "   OFFSET : %02x %02x (%lu)\n",rec[0],rec[1],rec[2],rec[3],mach_read_from_4(rec)
                                   ,rec[4],rec[5],mach_read_from_2(rec+4)
                                   ,rec[6],rec[7],rec[8],rec[9],mach_read_from_4(rec+6)
                                   ,rec[10],rec[11],mach_read_from_2(rec+10));
    return;
}
/*****************************************************************/
/**dump undo seg hdr*/
static 
void
btr_cur_dump_undo_seg_hdr
(
FILE*         fp_dmp_undo_seg_hdr,
const         page_t*   page)
{
    const byte*    rec=page;
    fprintf(fp_dmp_undo_seg_hdr,"--------------------------------\n");
    fprintf(fp_dmp_undo_seg_hdr,"TRX_UNDO_STATE(1-ACTIVE 2-CACHED 3-FREE 4-PURGE 5-PREPAID): %02x %02x \n",rec[0],rec[1]);
    rec+=2;
    fprintf(fp_dmp_undo_seg_hdr,"TRX_UNDO_LAST_LOG_HDR_ON_SEG_HDR:"
    " %02x %02x (%lu)\n",rec[0],rec[1],mach_read_from_2(rec));
    rec+=2;
    fprintf(fp_dmp_undo_seg_hdr,"TRX_UNDO_FSEG_HEADER(Header for the file segment which the undo log segment occupies):\n"
        "  SPACE_ID: %02x %02x %02x %02x (%lu)\n"
        "  PAGE_NO : %02x %02x %02x %02x (%lu)\n"
        "  OFFSET  : %02x %02x (%lu)\n",rec[0],rec[1],rec[2],rec[3],mach_read_from_4(rec)
                                       ,rec[4],rec[5],rec[6],rec[7],mach_read_from_4(rec+4)
                                       ,rec[8],rec[9],mach_read_from_2(rec+8));
    rec+=10;
    fprintf(fp_dmp_undo_seg_hdr,"TRX_UNDO_PAGE_LIST(Base node for the list of pages in the undo log segment; \n defined only on the undo log segment's first page):\n"
    "  FLS_LEN: %02x %02x %02x %02x (%lu)\n"
    "  FLS_FIRST:\n"
    "     PAGE_NO: %02x %02x %02x %02x (%lu)\n"
    "     OFFSET  : %02x %02x (%lu)\n"
    "  FLS_LAST:\n"
    "     PAGE_NO: %02x %02x %02x %02x (%lu)\n"
    "     OFFSET  : %02x %02x (%lu)\n",rec[0],rec[1],rec[2],rec[3],mach_read_from_4(rec)
                                   ,rec[4],rec[5],rec[6],rec[7],mach_read_from_4(rec+4)
                                   ,rec[8],rec[9],mach_read_from_2(rec+8)
                                   ,rec[10],rec[11],rec[12],rec[13],mach_read_from_4(rec+10)
                                   ,rec[14],rec[15],mach_read_from_2(rec+14));
    return;
}
/*****************************************************************/
/**dump undo seg hdr*/
static 
void
btr_cur_dump_undo_log_hdr
(
FILE*         fp_dmp_undo_log_hdr,
const         page_t*   page,
uint         idx)
{
    const byte*    rec=page;
    fprintf(fp_dmp_undo_log_hdr,"--------------------------------\n"
    "This is %u th undo log header: offset:%lu\n",idx,page_offset(rec));
    fprintf(fp_dmp_undo_log_hdr,"TRX_UNDO_TRX_ID:"
    " %02x %02x %02x %02x %02x %02x %02x %02x (%lu)\n",rec[0],rec[1],rec[2],rec[3]
                            ,rec[4],rec[5],rec[6],rec[7],mach_read_from_8(rec));
    rec+=8;
    
    fprintf(fp_dmp_undo_log_hdr,"TRX_UNDO_TRX_NO:"
    " %02x %02x %02x %02x %02x %02x %02x %02x (%lu)\n",rec[0],rec[1],rec[2],rec[3]
                            ,rec[4],rec[5],rec[6],rec[7],mach_read_from_8(rec));
    rec+=8;
    fprintf(fp_dmp_undo_log_hdr,"TRX_UNDO_DEL_MARKS:"
     " %02x %02x (%lu)\n",rec[0],rec[1],mach_read_from_2(rec));
    rec+=2;
    fprintf(fp_dmp_undo_log_hdr,"TRX_UNDO_LOG_START:"
     " %02x %02x (%lu)\n",rec[0],rec[1],mach_read_from_2(rec));
    rec+=2;
    fprintf(fp_dmp_undo_log_hdr,"TRX_UNDO_XID_EXISTS:"
     " %02x \n",rec[0]);
    rec+=1;
    fprintf(fp_dmp_undo_log_hdr,"TRX_UNDO_DICT_TRANS:"
     " %02x\n",rec[0]);
    rec+=1;
    fprintf(fp_dmp_undo_log_hdr,"TRX_UNDO_TABLE_ID:"
    " %02x %02x %02x %02x %02x %02x %02x %02x (%lu)\n",rec[0],rec[1],rec[2],rec[3]
                            ,rec[4],rec[5],rec[6],rec[7],mach_read_from_8(rec));
    rec+=8;
    fprintf(fp_dmp_undo_log_hdr,"TRX_UNDO_NEXT_LOG_HDR_ON_THIS_PAGE:"
    " %02x %02x (%lu)\n",rec[0],rec[1],mach_read_from_2(rec));
    rec+=2;
    fprintf(fp_dmp_undo_log_hdr,"TRX_UNDO_PREV_LOG_HDR_ON_THIS_PAGE:"
        " %02x %02x (%lu)\n",rec[0],rec[1],mach_read_from_2(rec));
    rec+=2;
    fprintf(fp_dmp_undo_log_hdr,"TRX_UNDO_HISTORY_NODE(If the log is put to the history list, the file list node is here):\n"
    "  HIS_FIL_PREV:\n"
    "     PAGE_NO: %02x %02x %02x %02x (%lu)\n"
    "     OFFSET  : %02x %02x (%lu)\n"
    "  HIS_FIL_NEXT:\n"
    "     PAGE_NO: %02x %02x %02x %02x (%lu)\n"
    "     OFFSET  : %02x %02x (%lu)\n",rec[0],rec[1],rec[2],rec[3],mach_read_from_4(rec)
                                   ,rec[4],rec[5],mach_read_from_2(rec+4)
                                   ,rec[6],rec[7],rec[8],rec[9],mach_read_from_4(rec+6)
                                   ,rec[10],rec[11],mach_read_from_2(rec+10));
    return;
}
/*****************************************************************/
/**get table from buf pool cache*/
static
dict_table_t *
btr_cur_get_table_if_in_cache(
table_id_t table_id)
{
 dict_table_t *table;
    if (dict_sys == NULL) 
    {
        return(NULL);
    }

    for (table = UT_LIST_GET_FIRST(dict_sys->table_LRU);
         table != NULL;
         table = UT_LIST_GET_NEXT(table_LRU, table)) 
    {
       if(table->id==table_id)
       {
         return table;
       }
    }

    for (table = UT_LIST_GET_FIRST(dict_sys->table_non_LRU);
         table != NULL;
         table = UT_LIST_GET_NEXT(table_LRU, table)) 
    {

        if(table->id==table_id)
        {
          return table;
        }

    }

    return(NULL);

}
/*****************************************************************/
/**dump one undo log delete record old value*/
static
void
btr_cur_dump_undo_log_delrec_orivalue
(
FILE*   fp_dmp_undo_delrec_ori,
const trx_undo_rec_t*  undo_rec,
dict_index_t         *index
)
{
    ulint        i;
    bool        first_v_col = true;
    bool        is_undo_log = true;
    ulint        n_skip_field = 0;
    const       byte*   ptr=undo_rec;
    const       byte*   end_ptr;
    ulint        del_offset=mach_read_from_2(ptr);
    end_ptr=ptr+del_offset;
    ptr+=2;
    for (i = 0; ptr < end_ptr; i++) 
    {

        const byte*    field;
        ulint        len;
        ulint        field_no;
        ulint        orig_len;
        bool        is_virtual;

        field_no = mach_read_next_compressed(&ptr);

        is_virtual = (field_no >= REC_MAX_N_FIELDS);

        if (is_virtual) 
        {
            /* If new version, we need to check index list to figure
            out the correct virtual column position */
            ptr = trx_undo_read_v_idx(
                index->table, ptr, first_v_col, &is_undo_log,
                &field_no);
            first_v_col = false;
        } 
        else if (field_no >= dict_index_get_n_fields(index)) 
        {
           
           return;
        }

        if (is_virtual) 
        {
            /* This column could be dropped or no longer indexed */
            if (field_no == ULINT_UNDEFINED) 
            {
                /* Mark this is no longer needed */
                ptr = trx_undo_rec_get_col_val(
                    ptr, &field, &len, &orig_len);
                ptr = trx_undo_rec_get_col_val(
                    ptr, &field, &len, &orig_len);
                n_skip_field++;
                continue;
            }
        } 
        ptr = trx_undo_rec_get_col_val(ptr, &field, &len, &orig_len);
        if (len == UNIV_SQL_NULL) {
           len=0;
        } else if (len < UNIV_EXTERN_STORAGE_FIELD) 
        {
          ;    
        } 
        else 
        {
            len -= UNIV_EXTERN_STORAGE_FIELD;
        }
        dict_field_t *d_field=dict_index_get_nth_field(index,field_no);
        fprintf(fp_dmp_undo_delrec_ori,
        "col %u: len:%lu  nm:[%s] \n",d_field->col->ind,len,(const char*)(d_field->name));
        ulint j=0;
        for(;j<len;)
        {
          fprintf(fp_dmp_undo_delrec_ori,
          " %02x",field[j++]);
          if(0==j%25)
          fprintf(fp_dmp_undo_delrec_ori,"\n");
        }
        if(j!=0)
        fprintf(fp_dmp_undo_delrec_ori,"\n");
        if (is_virtual) 
        {
            ptr = trx_undo_rec_get_col_val(
                ptr, &field, &len, &orig_len);
        }
    }
    fprintf(fp_dmp_undo_delrec_ori,"ord_part_len:%lu"
    "-----------------------\n",(ptr-undo_rec));
    return ;
}
/*****************************************************************/
/**dump one undo log update record old value*/
static
void
btr_cur_dump_undo_log_updrec_orivalue
(
FILE*   fp_dmp_undo_updrec_ori,
const trx_undo_rec_t*  undo_rec,
dict_index_t         *index,
ulint                cmpl_info
)
{
    ulint        n_fields;
    ulint        i;
    bool        first_v_col = true;
    bool        is_undo_log = true;
    ulint        n_skip_field = 0;
    const       byte*   ptr=undo_rec;
    const       byte*   old_ptr=undo_rec;
    n_fields = mach_read_next_compressed(&ptr);
    for (i = 0; i < n_fields; i++) 
    {

        const byte*    field;
        ulint        len;
        ulint        field_no;
        ulint        orig_len;
        bool        is_virtual;

        field_no = mach_read_next_compressed(&ptr);

        is_virtual = (field_no >= REC_MAX_N_FIELDS);

        if (is_virtual) 
        {
            /* If new version, we need to check index list to figure
            out the correct virtual column position */
            ptr = trx_undo_read_v_idx(
                index->table, ptr, first_v_col, &is_undo_log,
                &field_no);
            first_v_col = false;
        } 
        else if (field_no >= dict_index_get_n_fields(index)) 
        {
           
           return;
        }

        if (is_virtual) 
        {
            /* This column could be dropped or no longer indexed */
            if (field_no == ULINT_UNDEFINED) 
            {
                /* Mark this is no longer needed */
                ptr = trx_undo_rec_get_col_val(
                    ptr, &field, &len, &orig_len);
                ptr = trx_undo_rec_get_col_val(
                    ptr, &field, &len, &orig_len);
                n_skip_field++;
                continue;
            }
        } 
        ptr = trx_undo_rec_get_col_val(ptr, &field, &len, &orig_len);
        if (len == UNIV_SQL_NULL) {
           len=0;
        } else if (len < UNIV_EXTERN_STORAGE_FIELD) 
        {
          ;    
        } 
        else 
        {
            len -= UNIV_EXTERN_STORAGE_FIELD;
        }
        dict_field_t *d_field=dict_index_get_nth_field(index,field_no);
        fprintf(fp_dmp_undo_updrec_ori,
        "col %u: len:%lu  nm:[%s] \n",d_field->col->ind,len,(const char*)(d_field->name));
        ulint j=0;
        for(;j<len;)
        {
          fprintf(fp_dmp_undo_updrec_ori,
          " %02x",field[j++]);
          if(0==j%25)
          fprintf(fp_dmp_undo_updrec_ori,"\n");
        }
        if(j!=0)
        fprintf(fp_dmp_undo_updrec_ori,"\n");
        if (is_virtual) 
        {
            ptr = trx_undo_rec_get_col_val(
                ptr, &field, &len, &orig_len);
        }
    }
    fprintf(fp_dmp_undo_updrec_ori," orig_col_len:%lu \n"
                                   "-----------------------\n",(ptr-old_ptr));
    old_ptr=ptr;
    if(!(cmpl_info & UPD_NODE_NO_ORD_CHANGE))
    {
      const ulint len=mach_read_from_2(ptr);
      const byte* end_ptr=ptr+len;
      ptr+=2;
      while(ptr<end_ptr)
      {
        const byte*    field;
        ulint        len;
        ulint        pos;
        ulint        orig_len;
        pos=mach_read_next_compressed(&ptr);
        ptr = trx_undo_rec_get_col_val(ptr, &field, &len, &orig_len);
        if (len == UNIV_SQL_NULL) {
           len=0;
        } else if (len < UNIV_EXTERN_STORAGE_FIELD) 
        {
          ;    
        } 
        else 
        {
            len -= UNIV_EXTERN_STORAGE_FIELD;
        }
        dict_field_t *d_field=dict_index_get_nth_field(index,pos);
        if(d_field)
        {
            fprintf(fp_dmp_undo_updrec_ori,
              "col %u: pos:%lu len:%lu  nm:[%s] \n",d_field->col->ind,pos,len,(const char*)(d_field->name));
        }
        else
        {
            fprintf(fp_dmp_undo_updrec_ori,
              "col: pos:%lu len:%lu \n",pos,len);
        }
        ulint j=0;
        for(;j<len;)
        {
          fprintf(fp_dmp_undo_updrec_ori,
          " %02x",field[j++]);
          if(0==j%25)
          fprintf(fp_dmp_undo_updrec_ori,"\n");
        }
        if(j!=0)
        fprintf(fp_dmp_undo_updrec_ori,"\n");
      }
      fprintf(fp_dmp_undo_updrec_ori,"ord_part_len:%lu \n"
                                   "-----------------------\n",(ptr-old_ptr));
    }
    return ;
}
/*****************************************************************/
/**dump one undo log update record*/
static
void
btr_cur_dump_undo_log_upd_record
(
FILE*   fp_dmp_undo_upd_rec,
const trx_undo_rec_t*  undo_rec,
table_id_t            table_id,
ulint                 upd_type,
ulint                 cmpl_info
)
{
    byte byte_buf[32]={0};
    const byte* read_rec=undo_rec;
    const byte* old_ptr=undo_rec;
    fprintf(fp_dmp_undo_upd_rec, 
    " INFO_BITS: %u%u%u%u%u%u%u%u \n",(1&(read_rec[0]))
                                             ,(1&(read_rec[0]>>1))
                                             ,(1&(read_rec[0]>>2))
                                             ,(1&(read_rec[0]>>3))
                                             ,(1&(read_rec[0]>>4))
                                             ,(1&(read_rec[0]>>5))
                                             ,(1&(read_rec[0]>>6))
                                             ,(1&(read_rec[0]>>7))
                                             );
    read_rec++;
    trx_id_t   trx_id=mach_u64_read_next_compressed(&read_rec);         
    mach_write_to_6(byte_buf,trx_id);
    roll_ptr_t  roll_ptr=mach_u64_read_next_compressed(&read_rec);
    mach_write_to_7(byte_buf+6,roll_ptr);
    ibool  is_insert;
    ulint  rseg_id;
    ulint  page_no;
    ulint  roll_offset; 
    trx_undo_decode_roll_ptr(roll_ptr,&is_insert,&rseg_id,&page_no,&roll_offset);
    fprintf(fp_dmp_undo_upd_rec,
    " TRX_ID: %02x %02x %02x %02x %02x %02x (%lu)\n",byte_buf[0],byte_buf[1],byte_buf[2]
                                                    ,byte_buf[3],byte_buf[4],byte_buf[5],trx_id);
    fprintf(fp_dmp_undo_upd_rec,
    " POLL_PTR: %02x %02x %02x %02x %02x %02x %02x (%lu %lu %lu %lu %lu)\n",byte_buf[6],byte_buf[7],byte_buf[8]
                                                    ,byte_buf[9],byte_buf[10],byte_buf[11]
                                                    ,byte_buf[12],rseg_id,is_insert,trx_undo_get_space_id(rseg_id),page_no,roll_offset);
    fprintf(fp_dmp_undo_upd_rec,"compressed_len:%lu\n"
                                "-----------------------\n",(read_rec-old_ptr));
    dict_table_t   *table;
    dict_index_t   *cls_index;
    table=btr_cur_get_table_if_in_cache(table_id);
    if(NULL==table)
    {
      fprintf(fp_dmp_undo_upd_rec,"table is %lu is not exist!\n",table_id);
      return;
    }
    cls_index=dict_table_get_first_index(table);
    if(NULL==cls_index)
    {
       fprintf(fp_dmp_undo_upd_rec,"table %lu index is not exist!\n",table_id);
       return;
    }
    ulint u_unqs=dict_index_get_n_unique(cls_index);
    old_ptr=read_rec;
    for(ulint i=0;i<u_unqs;i++)
    {
          ulint       flen=mach_read_next_compressed(&read_rec);
          if(flen!=UNIV_SQL_NULL)
          {
            ulint j=0;
            dict_field_t  *d_field=dict_index_get_nth_field(cls_index,i);
            fprintf(fp_dmp_undo_upd_rec,
            " pk[%lu]:  len:%lu nm:[%s]\n",i,flen,(const char*)(d_field->name));
            for(;j<flen;)
            {
              fprintf(fp_dmp_undo_upd_rec," %02x",read_rec[j++]);
              if(0==j%25)
              fprintf(fp_dmp_undo_upd_rec,"\n");
            }
            if(j!=0||0==flen)
            fprintf(fp_dmp_undo_upd_rec,"\n");
            read_rec+=flen;
          }
          else
          {
            fprintf(fp_dmp_undo_upd_rec,
            " pk[%lu]:  len:0 \n",i);
          }
    }
    fprintf(fp_dmp_undo_upd_rec,"unqs_len:%lu \n"
                                "-----------------------\n",(read_rec-old_ptr));
    if(upd_type==TRX_UNDO_DEL_MARK_REC)
    {
      btr_cur_dump_undo_log_delrec_orivalue(fp_dmp_undo_upd_rec,read_rec,cls_index); 
    }
    else
    {
        btr_cur_dump_undo_log_updrec_orivalue(fp_dmp_undo_upd_rec,read_rec,cls_index,cmpl_info);
    }
    return;
}
/*****************************************************************/
/**dump one undo log insert record*/
static
void
btr_cur_dump_undo_log_ins_record
(
FILE*   fp_dmp_undo_ins_rec,
const trx_undo_rec_t*  undo_rec,
table_id_t            table_id
)
{

    const byte* read_rec=undo_rec;
    dict_table_t   *table;
    dict_index_t   *cls_index;
    table=btr_cur_get_table_if_in_cache(table_id);
    if(NULL==table)
    {
      fprintf(fp_dmp_undo_ins_rec,"table is %lu is not exist!\n",table_id);
      return;
    }
    cls_index=dict_table_get_first_index(table);
    if(NULL==cls_index)
    {
       fprintf(fp_dmp_undo_ins_rec,"table %lu index is not exist!\n",table_id);
       return;
    }
    ulint u_unqs=dict_index_get_n_unique(cls_index);
    for(uint i=0;i<u_unqs;i++)
    {
          ulint       flen=mach_read_next_compressed(&read_rec);
          if(flen!=UNIV_SQL_NULL)
          {
            ulint j=0;
            dict_field_t  *d_field=dict_index_get_nth_field(cls_index,i);
            fprintf(fp_dmp_undo_ins_rec,
            " pk[%u]:  len:%lu nm:[%s] \n",i,flen,(const char*)(d_field->name));
            for(;j<flen;)
            {
              fprintf(fp_dmp_undo_ins_rec," %02x",read_rec[j++]);
              if(0==j%25)
              fprintf(fp_dmp_undo_ins_rec,"\n");
            }
            if(j!=0||0==flen)
            fprintf(fp_dmp_undo_ins_rec,"\n");
            read_rec+=flen;
          }
          else
          {
            fprintf(fp_dmp_undo_ins_rec,
            " pk[%u]:  len:0 \n",i);
          }
    }
    fprintf(fp_dmp_undo_ins_rec,"unqs_len:%lu \n"
                                "-----------------------\n",(read_rec-undo_rec));
    return;
}
/*****************************************************************/
/**dump undo log records*/
static
void
btr_cur_dump_undo_log_hdr_records
(
FILE*      fp_dmp_undo_records,
page_t*  undo_page,
ulint          page_no,
ulint          loghdr_offset
)
{
    trx_undo_rec_t    *current_rec=NULL,*read_rec=NULL;
    ulint     current_offset;
    fprintf(fp_dmp_undo_records,"\nundo log records:\n");
    uint i=0;
    for(
        current_rec = trx_undo_page_get_first_rec(undo_page, page_no, loghdr_offset);
        current_rec !=NULL;
        current_rec = trx_undo_page_get_next_rec(current_rec,page_no,loghdr_offset)
        )
    {
        ulint  rec_type,cmpl_info;
        undo_no_t   undo_no;
        table_id_t  table_id;
        bool        is_updated_extern;
        current_offset=mach_read_from_2(current_rec);
        read_rec=trx_undo_rec_get_pars(current_rec,&rec_type,&cmpl_info,
                   &is_updated_extern,&undo_no,&table_id);
        fprintf(fp_dmp_undo_records,
        "row#%u : offset:%lu next_rec:%lu  same_cols_len:%lu\n"
         " type(11-i 12-upde 13-upddel 14-delmark 16-cmplmult ):%lu undo_no:%lu table_id:%lu \n"
         ,i++,page_offset(current_rec),current_offset,(read_rec-current_rec),rec_type,(ulint)undo_no,(ulint)table_id);
        if(TRX_UNDO_INSERT_REC==rec_type)
        {
          btr_cur_dump_undo_log_ins_record(fp_dmp_undo_records,read_rec,table_id);
        }
        else
        {
          btr_cur_dump_undo_log_upd_record(fp_dmp_undo_records,read_rec,table_id,rec_type,cmpl_info);
        }
    }
    return;
}
/*****************************************************************/
/**dump undo log architecture*/
void
btr_cur_dump_undo_architecture
(  ulint         space_id,
ulint         page_no,
FILE*         fp_dmp_undo_arch)
{
   buf_block_t*    block=NULL;
   page_t*         page;
   mtr_t           mtr;

   page_id_t  page_id(space_id,page_no);
   bool            found;
   const page_size_t&    page_size
       = fil_space_get_page_size(page_id.space(), &found);
   if(false==found)
   {
      fprintf(fp_dmp_undo_arch,"space_id=%ld is not exist\n",space_id);
      return;
   }
   mtr_start(&mtr);
   block = buf_page_get_gen(page_id, page_size, RW_NO_LATCH, NULL,
                    BUF_GET, __FILE__, __LINE__, &mtr);
   if(NULL==block)
   {
     fprintf(fp_dmp_undo_arch,"space_id=%ld page_no=%ld is not exist\n",space_id,page_no);
     return;
   }
   page = buf_block_get_frame(block);
   if(FIL_PAGE_TYPE_TRX_SYS != fil_page_get_type(page))
   {
      fprintf(fp_dmp_undo_arch,"File page type is not Transaction system data node \n");
      return;
   }
   btr_cur_dump_file_header(fp_dmp_undo_arch,page);
   btr_cur_dump_undo_mngr_hdr(fp_dmp_undo_arch,page+FIL_PAGE_DATA);
   fprintf(fp_dmp_undo_arch,"--------------------------------\n");
   return;    
}
/*****************************************************************/
/**dump rollback segment inode*/
void
btr_cur_dump_rseg_inode
(  ulint         space_id,
ulint         page_no,
FILE*         fp_dmp_rseg
)
{
    buf_block_t*    block=NULL;
    page_t*         page;
    mtr_t           mtr;
    
    page_id_t  page_id(space_id,page_no);
    bool            found;
    const page_size_t&    page_size
        = fil_space_get_page_size(page_id.space(), &found);
   if(false==found)
   {
      fprintf(fp_dmp_rseg,"space_id=%ld is not exist\n",space_id);
      return;
   }
    mtr_start(&mtr);
    block = buf_page_get_gen(page_id, page_size, RW_NO_LATCH, NULL,
                     BUF_GET, __FILE__, __LINE__, &mtr);
    if(NULL==block)
    {
      fprintf(fp_dmp_rseg,"space_id=%ld page_no=%ld is not exist\n",space_id,page_no);
      return;
    }
    page = buf_block_get_frame(block);
    if(FIL_PAGE_TYPE_SYS != fil_page_get_type(page))
    {
       fprintf(fp_dmp_rseg,"File page type is not rollback segment system data node \n");
       return;
    }
    btr_cur_dump_file_header(fp_dmp_rseg,page);
    btr_cur_dump_rseg_hdr(fp_dmp_rseg,page+FIL_PAGE_DATA);
    fprintf(fp_dmp_rseg,"--------------------------------\n");
    return;    
}
/*****************************************************************/
/**dump undo log segment inode*/
void
btr_cur_dump_undolog_inode
(  ulint         space_id,
ulint         page_no,
FILE*         fp_dmp_undolog)
{
    buf_block_t*    block=NULL;
    page_t*         undo_page;
    mtr_t           mtr;
    
    page_id_t  page_id(space_id,page_no);
    bool            found;
    const page_size_t&    page_size
        = fil_space_get_page_size(page_id.space(), &found);
   if(false==found)
   {
      fprintf(fp_dmp_undolog,"space_id=%ld is not exist\n",space_id);
      return;
   }
    mtr_start(&mtr);
    block = buf_page_get_gen(page_id, page_size, RW_NO_LATCH, NULL,
                     BUF_GET, __FILE__, __LINE__, &mtr);
    if(NULL==block)
    {
      fprintf(fp_dmp_undolog,"space_id=%ld page_no=%ld is not exist\n",space_id,page_no);
      return;
    }
    undo_page = buf_block_get_frame(block);
    if(FIL_PAGE_UNDO_LOG != fil_page_get_type(undo_page))
    {
       fprintf(fp_dmp_undolog,"File page type is not undo log data node \n");
       return;
    }
    //mtr_start(&mtr1);
    ulint prev_page_no=mach_read_from_4(undo_page+TRX_UNDO_PAGE_HDR+TRX_UNDO_PAGE_NODE);
    if(prev_page_no==FIL_NULL)
    {/*undo log hdr page*/
      btr_cur_dump_file_header(fp_dmp_undolog,undo_page);
      btr_cur_dump_undo_page_hdr(fp_dmp_undolog,undo_page+TRX_UNDO_PAGE_HDR);
      btr_cur_dump_undo_seg_hdr(fp_dmp_undolog,undo_page+TRX_UNDO_SEG_HDR);
      ulint  log_hdr_offset=TRX_UNDO_SEG_HDR+TRX_UNDO_SEG_HDR_SIZE;
      const  byte*  log_hdr_rec;
      for(uint i=0;log_hdr_offset!=0 ;)
      {
        log_hdr_rec=undo_page+log_hdr_offset;
        btr_cur_dump_undo_log_hdr(fp_dmp_undolog,undo_page+log_hdr_offset,i);
        btr_cur_dump_undo_log_hdr_records(fp_dmp_undolog,undo_page,page_get_page_no(undo_page),log_hdr_offset);
        log_hdr_offset=mach_read_from_2(log_hdr_rec+TRX_UNDO_NEXT_LOG);
        i++;
      } 

    }
    else
    {
      btr_cur_dump_file_header(fp_dmp_undolog,undo_page);
      btr_cur_dump_undo_page_hdr(fp_dmp_undolog,undo_page+TRX_UNDO_PAGE_HDR);
      btr_cur_dump_undo_log_hdr_records(fp_dmp_undolog,undo_page,prev_page_no,0);
    }
    fprintf(fp_dmp_undolog,"--------------------------------\n");
    mtr_commit(&mtr);
    //mtr_commit(&mtr1);
    return;    
}
/*************************************************
 dump redo log hdr
 ************************************************/
void log_dump_redo_loghdr(
    FILE* fp_dmp_redo_hdr)
{
    log_mutex_enter();
    log_group_t*    group;
    group = UT_LIST_GET_FIRST(log_sys->log_groups);
    ulint        field;
    byte*        rec=log_sys->checkpoint_buf;
    field=0;
    log_group_header_read(group,field);
    /*fil_io(IORequestLogRead, false,
               page_id_t(group->space_id, field / univ_page_size.physical()),
               univ_page_size, field % univ_page_size.physical(),
               OS_FILE_LOG_BLOCK_SIZE, buf, NULL);*/
    fprintf(fp_dmp_redo_hdr,"--------------------------------\n"
    "LOG FILE HEADER:\n"
    "0 th log file header page\n");
    fprintf(fp_dmp_redo_hdr,"LOG_HEADER_FORMAT: %02x %02x %02x %02x (%ld)\n",rec[0],rec[1],rec[2],rec[3],mach_read_from_4(rec));
    rec+=4;
    fprintf(fp_dmp_redo_hdr,"LOG_HEADER_PAD1: %02x %02x %02x %02x (%ld) \n",rec[0],rec[1],rec[2],rec[3],mach_read_from_4(rec));
    rec+=4;
    fprintf(fp_dmp_redo_hdr,"LOG_HEADER_START_LSN: lsn of the start of data in this log file (with format version 1) \n"
                            " %02x %02x %02x %02x %02x %02x %02x %02x (%ld) \n",
                              rec[0],rec[1],rec[2],rec[3],rec[4],rec[5],rec[6],rec[7],mach_read_from_8(rec));
    rec+=8;
    fprintf(fp_dmp_redo_hdr,"LOG_HEADER_CREATOR: %32.32s\n",rec);

    field=LOG_CHECKPOINT_1;
    rec=log_sys->checkpoint_buf;
    log_group_header_read(group,field);
    /*fil_io(IORequestLogRead, false,
           page_id_t(group->space_id, field / univ_page_size.physical()),
           univ_page_size, field % univ_page_size.physical(),
           OS_FILE_LOG_BLOCK_SIZE, buf, NULL);*/
    fprintf(fp_dmp_redo_hdr,"----------------\n"
    "1 th log file header page\n");
    //rec=buf;
    fprintf(fp_dmp_redo_hdr,"LOG_CHECKPOINT_NO: %02x %02x %02x %02x %02x %02x %02x %02x(%ld) \n",
              rec[0],rec[1],rec[2],rec[3],rec[4],rec[5],rec[6],rec[7],mach_read_from_8(rec));
    rec+=8;
    fprintf(fp_dmp_redo_hdr,"LOG_CHECKPOINT_LSN: %02x %02x %02x %02x %02x %02x %02x %02x(%ld)\n",
              rec[0],rec[1],rec[2],rec[3],rec[4],rec[5],rec[6],rec[7],mach_read_from_8(rec));
    rec+=8;
    fprintf(fp_dmp_redo_hdr,"LOG_CHECKPOINT_OFFSET: %02x %02x %02x %02x %02x %02x %02x %02x(%ld) \n",
              rec[0],rec[1],rec[2],rec[3],rec[4],rec[5],rec[6],rec[7],mach_read_from_8(rec));
    rec+=8;
    fprintf(fp_dmp_redo_hdr,"LOG_CHECKPOINT_LOG_BUF_SIZE: %02x %02x %02x %02x %02x %02x %02x %02x(%ld) \n",
              rec[0],rec[1],rec[2],rec[3],rec[4],rec[5],rec[6],rec[7],mach_read_from_8(rec));
    rec+=8;
    
    field=LOG_CHECKPOINT_2;
    rec=log_sys->checkpoint_buf;
    log_group_header_read(group,field);
    /*fil_io(IORequestLogRead, false,
           page_id_t(group->space_id, field / univ_page_size.physical()),
           univ_page_size, field % univ_page_size.physical(),
           OS_FILE_LOG_BLOCK_SIZE, buf, NULL);*/
    fprintf(fp_dmp_redo_hdr,"----------------\n"
    "3 th log file header page\n");
    //rec=buf;
    fprintf(fp_dmp_redo_hdr,"LOG_CHECKPOINT_NO: %02x %02x %02x %02x %02x %02x %02x %02x(%ld) \n",
              rec[0],rec[1],rec[2],rec[3],rec[4],rec[5],rec[6],rec[7],mach_read_from_8(rec));
    rec+=8;
    fprintf(fp_dmp_redo_hdr,"LOG_CHECKPOINT_LSN: %02x %02x %02x %02x %02x %02x %02x %02x(%ld)\n",
              rec[0],rec[1],rec[2],rec[3],rec[4],rec[5],rec[6],rec[7],mach_read_from_8(rec));
    rec+=8;
    fprintf(fp_dmp_redo_hdr,"LOG_CHECKPOINT_OFFSET: %02x %02x %02x %02x %02x %02x %02x %02x(%ld) \n",
              rec[0],rec[1],rec[2],rec[3],rec[4],rec[5],rec[6],rec[7],mach_read_from_8(rec));
    rec+=8;
    fprintf(fp_dmp_redo_hdr,"LOG_CHECKPOINT_LOG_BUF_SIZE: %02x %02x %02x %02x %02x %02x %02x %02x(%ld) \n",
              rec[0],rec[1],rec[2],rec[3],rec[4],rec[5],rec[6],rec[7],mach_read_from_8(rec));
    rec+=8;
    fprintf(fp_dmp_redo_hdr,"--------------------------------\n");
    log_mutex_exit();
    return;  
}

static 
const char*
btr_get_mlog_string(mlog_id_t type)
{
    switch (type) {
    case MLOG_SINGLE_REC_FLAG:
        return("MLOG_SINGLE_REC_FLAG");

    case MLOG_1BYTE:
        return("MLOG_1BYTE");

    case MLOG_2BYTES:
        return("MLOG_2BYTES");

    case MLOG_4BYTES:
        return("MLOG_4BYTES");

    case MLOG_8BYTES:
        return("MLOG_8BYTES");

    case MLOG_REC_INSERT:
        return("MLOG_REC_INSERT");

    case MLOG_REC_CLUST_DELETE_MARK:
        return("MLOG_REC_CLUST_DELETE_MARK");

    case MLOG_REC_SEC_DELETE_MARK:
        return("MLOG_REC_SEC_DELETE_MARK");

    case MLOG_REC_UPDATE_IN_PLACE:
        return("MLOG_REC_UPDATE_IN_PLACE");

    case MLOG_REC_DELETE:
        return("MLOG_REC_DELETE");

    case MLOG_LIST_END_DELETE:
        return("MLOG_LIST_END_DELETE");

    case MLOG_LIST_START_DELETE:
        return("MLOG_LIST_START_DELETE");

    case MLOG_LIST_END_COPY_CREATED:
        return("MLOG_LIST_END_COPY_CREATED");

    case MLOG_PAGE_REORGANIZE:
        return("MLOG_PAGE_REORGANIZE");

    case MLOG_PAGE_CREATE:
        return("MLOG_PAGE_CREATE");

    case MLOG_UNDO_INSERT:
        return("MLOG_UNDO_INSERT");

    case MLOG_UNDO_ERASE_END:
        return("MLOG_UNDO_ERASE_END");

    case MLOG_UNDO_INIT:
        return("MLOG_UNDO_INIT");

    case MLOG_UNDO_HDR_DISCARD:
        return("MLOG_UNDO_HDR_DISCARD");

    case MLOG_UNDO_HDR_REUSE:
        return("MLOG_UNDO_HDR_REUSE");

    case MLOG_UNDO_HDR_CREATE:
        return("MLOG_UNDO_HDR_CREATE");

    case MLOG_REC_MIN_MARK:
        return("MLOG_REC_MIN_MARK");

    case MLOG_IBUF_BITMAP_INIT:
        return("MLOG_IBUF_BITMAP_INIT");

#ifdef UNIV_LOG_LSN_DEBUG
    case MLOG_LSN:
        return("MLOG_LSN");
#endif /* UNIV_LOG_LSN_DEBUG */

    case MLOG_INIT_FILE_PAGE:
        return("MLOG_INIT_FILE_PAGE");

    case MLOG_WRITE_STRING:
        return("MLOG_WRITE_STRING");

    case MLOG_MULTI_REC_END:
        return("MLOG_MULTI_REC_END");

    case MLOG_DUMMY_RECORD:
        return("MLOG_DUMMY_RECORD");

    case MLOG_FILE_DELETE:
        return("MLOG_FILE_DELETE");

    case MLOG_COMP_REC_MIN_MARK:
        return("MLOG_COMP_REC_MIN_MARK");

    case MLOG_COMP_PAGE_CREATE:
        return("MLOG_COMP_PAGE_CREATE");

    case MLOG_COMP_REC_INSERT:
        return("MLOG_COMP_REC_INSERT");

    case MLOG_COMP_REC_CLUST_DELETE_MARK:
        return("MLOG_COMP_REC_CLUST_DELETE_MARK");

    case MLOG_COMP_REC_SEC_DELETE_MARK:
        return("MLOG_COMP_REC_SEC_DELETE_MARK");

    case MLOG_COMP_REC_UPDATE_IN_PLACE:
        return("MLOG_COMP_REC_UPDATE_IN_PLACE");

    case MLOG_COMP_REC_DELETE:
        return("MLOG_COMP_REC_DELETE");

    case MLOG_COMP_LIST_END_DELETE:
        return("MLOG_COMP_LIST_END_DELETE");

    case MLOG_COMP_LIST_START_DELETE:
        return("MLOG_COMP_LIST_START_DELETE");

    case MLOG_COMP_LIST_END_COPY_CREATED:
        return("MLOG_COMP_LIST_END_COPY_CREATED");

    case MLOG_COMP_PAGE_REORGANIZE:
        return("MLOG_COMP_PAGE_REORGANIZE");

    case MLOG_FILE_CREATE2:
        return("MLOG_FILE_CREATE2");

    case MLOG_ZIP_WRITE_NODE_PTR:
        return("MLOG_ZIP_WRITE_NODE_PTR");

    case MLOG_ZIP_WRITE_BLOB_PTR:
        return("MLOG_ZIP_WRITE_BLOB_PTR");

    case MLOG_ZIP_WRITE_HEADER:
        return("MLOG_ZIP_WRITE_HEADER");

    case MLOG_ZIP_PAGE_COMPRESS:
        return("MLOG_ZIP_PAGE_COMPRESS");

    case MLOG_ZIP_PAGE_COMPRESS_NO_DATA:
        return("MLOG_ZIP_PAGE_COMPRESS_NO_DATA");

    case MLOG_ZIP_PAGE_REORGANIZE:
        return("MLOG_ZIP_PAGE_REORGANIZE");

    case MLOG_FILE_RENAME2:
        return("MLOG_FILE_RENAME2");

    case MLOG_FILE_NAME:
        return("MLOG_FILE_NAME");

    case MLOG_CHECKPOINT:
        return("MLOG_CHECKPOINT");

    case MLOG_PAGE_CREATE_RTREE:
        return("MLOG_PAGE_CREATE_RTREE");

    case MLOG_COMP_PAGE_CREATE_RTREE:
        return("MLOG_COMP_PAGE_CREATE_RTREE");

    case MLOG_INIT_FILE_PAGE2:
        return("MLOG_INIT_FILE_PAGE2");

    case MLOG_INDEX_LOAD:
        return("MLOG_INDEX_LOAD");

    case MLOG_TRUNCATE:
        return("MLOG_TRUNCATE");
    }
    return(NULL);
}
/****************************************
 btr_cur_fil_name_parse
 ***************************************/
static
byte*
btr_cur_fil_name_parse(
       FILE*           fp_dump_logrec,
  byte*   ptr,
  const byte* end,
  mlog_id_t type)
{
  if (type == MLOG_FILE_CREATE2) {
    if (end < ptr + 4) {
      return(NULL);
    }
    fprintf(fp_dump_logrec,"flags=%ld \n",mach_read_from_4(ptr));
    ptr += 4;
  }

  if (end < ptr + 2) {
    return(NULL);
  }

  ulint len = mach_read_from_2(ptr);
  ptr += 2;
  byte fn_redo[512]={0};
  memcpy(fn_redo, ptr, len-1);
  fprintf(fp_dump_logrec,"len=%ld   fn=[%s] \n",len,(char*)fn_redo);
  if (end < ptr + len) {
    return(NULL);
  }
      byte* end_ptr = ptr + len;
  if(type==MLOG_FILE_RENAME2)
  {
    byte* new_name = end_ptr + 2;
    if (end < new_name) {
      return(NULL);
    }
    ulint new_len = mach_read_from_2(end_ptr);
             memset(fn_redo,0x00,sizeof(fn_redo));
        memcpy(fn_redo, new_name, new_len);
    fprintf(fp_dump_logrec,"new_len=%ld   new_fn=[%s] \n",new_len,(char*)fn_redo);
    if (end < end_ptr + 2 + new_len) {
      return(NULL);
    }

    end_ptr += 2 + new_len;
    }
   return(end_ptr);
  
}
/****************************************
 btr_cur_print_index
 ***************************************/
static
void
btr_cur_print_index(
FILE*           fp_dump_logrec,
dict_index_t* index,
ibool        comp
)
{
  if(NULL==index ||NULL==index->table)
   {return;}
  fprintf(fp_dump_logrec,"n_fields:%u  n_uniq:%u \n",index->n_fields,index->n_uniq);
  if(!comp)
    return;
  for(ulint i=0;i<index->n_fields;i++ )
  {
    fprintf(fp_dump_logrec,"field[%ld].fixed_len=%u \n",i,index->fields[i].fixed_len);
  }
  return;
}
/***********************************************************//**
Parses a redo log record of updating a record in-place.
@return end of log record or NULL */
static 
byte*
btr_cur_parse_log_update_in_place(
 FILE*    fp_dump_logrec_upd,
    byte*   ptr,
    byte*   end_ptr
)
{
    ulint        flags;
    
    upd_t*        update;
    ulint        pos;
    trx_id_t    trx_id;
    roll_ptr_t    roll_ptr;
    ulint        rec_offset;
    mem_heap_t*    heap;
    

    if (end_ptr < ptr + 1)
    {
        return(NULL);
    }
    flags = mach_read_from_1(ptr);
    ptr++;
    ptr = row_upd_parse_sys_vals(ptr, end_ptr, &pos, &trx_id, &roll_ptr);
    if (ptr == NULL) {
        return(NULL);
    }
    if (end_ptr < ptr + 2) {
        return(NULL);
    }
    rec_offset = mach_read_from_2(ptr);
    ptr += 2;
    heap = mem_heap_create(256);
    ptr = row_upd_index_parse(ptr, end_ptr, heap, &update);
    if(!ptr)
    {
      mem_heap_free(heap);
      return(NULL);
    }
    fprintf(fp_dump_logrec_upd,"flags:%ld trx_id_pos:%ld  trx_id:%ld roll_ptr:%ld page_offset:%ld \n",
           flags,pos,trx_id,roll_ptr,rec_offset);
    fprintf(fp_dump_logrec_upd,"update details: \n"
                               "info_bits:%ld updated_n_fields:%ld \n",
                               update->info_bits,update->n_fields);
    for(ulint i=0;i<update->n_fields;i++)
    {
      fprintf(fp_dump_logrec_upd,"field_no:%u ",update->fields[i].field_no);
      ulint newval_len=update->fields[i].new_val.len;
      if(UNIV_SQL_NULL!=newval_len)
      {
         fprintf(fp_dump_logrec_upd,"newval_len:%ld \n"
                                    "newvalue:\n",newval_len);
         byte* newdata=(byte*)update->fields[i].new_val.data;
         for(ulint j=0;j<newval_len;)
         {
           fprintf(fp_dump_logrec_upd," %02x",newdata[j++]);
           if(0==j%32)
            fprintf(fp_dump_logrec_upd,"\n");
         }
         fprintf(fp_dump_logrec_upd,"\n");
      }
      else
      {
          fprintf(fp_dump_logrec_upd,"newval_len:%ld (UNIV_SQL_NULL) \n",newval_len);
      }
    }
    mem_heap_free(heap);
    return(ptr);

}
/****************************************
 btr_cur_parse_log_rec_body
 ***************************************/
static
byte*
btr_cur_parse_log_rec_body(
  FILE*           fp_dump_logrec,
  mlog_id_t type,
  byte*   ptr,
  byte*   end_ptr,
  ulint   space_id,
  ulint   page_no,
  buf_block_t*  block,
  mtr_t*    mtr)
{
  switch (type) 
  {
  case MLOG_FILE_NAME:
  case MLOG_FILE_DELETE:
  case MLOG_FILE_CREATE2:
  case MLOG_FILE_RENAME2:
    return(btr_cur_fil_name_parse(fp_dump_logrec,ptr,end_ptr,type));

  case MLOG_INDEX_LOAD:
  if (end_ptr < ptr + 8) 
  {
      return(NULL);
  }
  fprintf(fp_dump_logrec,"len=9  index_id=%ld \n",mach_read_from_8(ptr));
  return(ptr + 8);
  
  case MLOG_TRUNCATE:
    fprintf(fp_dump_logrec,"lsn=%ld \n",mach_read_from_8(ptr));
    return(ptr+8);
  case MLOG_WRITE_STRING:
    if (page_no == 0 && !is_system_tablespace(space_id)) 
    {
      ulint   offset;
      ulint   len;
      offset = mach_read_from_2(ptr);
      ptr += 2;
      len = mach_read_from_2(ptr);
      ptr += 2;
      if (end_ptr< ptr + len) 
      {
         return(NULL);
      }
      byte fnstring[512]={0};
      memcpy(fnstring,ptr,len);
      fprintf(fp_dump_logrec,"page_offset=%ld   len=%ld  string=%s \n",
            offset,len,(char*)fnstring);
      ptr+=len;
       return(ptr);
    }
    break;
  default:
    break;
  }
  dict_index_t* index = NULL;
  page_t*   page;
  page_zip_des_t* page_zip;
  /* Parsing a page log record. */
  page = NULL;
  page_zip = NULL;
  switch (type) 
  {
     case MLOG_1BYTE: case MLOG_2BYTES: case MLOG_4BYTES: case MLOG_8BYTES:
     {
       ulint   offset;
       ulint            dval;
       if (end_ptr < ptr + 2) {
       
         return(NULL);
       }
       offset = mach_read_from_2(ptr);
       ptr += 2;
       if (offset >= UNIV_PAGE_SIZE) {
         return(NULL);
       }
       
       if (type == MLOG_8BYTES) 
       {
         dval = mach_u64_parse_compressed((const byte**)&ptr, (const byte*)end_ptr);
         if (ptr == NULL) {
           return(NULL);
         }
       }
       else
       {
           dval = mach_parse_compressed((const byte**)&ptr, (const byte*)end_ptr);
           if (ptr == NULL) {
             return(NULL);
           }
       }
       fprintf(fp_dump_logrec,"page_offset=%ld  val=%ld \n",offset,dval);
     }
     break;
     
     case MLOG_REC_INSERT: case MLOG_COMP_REC_INSERT:
       if (NULL != (ptr = mlog_parse_index(
                ptr, end_ptr,
                type == MLOG_COMP_REC_INSERT,
                &index))) 
         {
            btr_cur_print_index(fp_dump_logrec,index,type == MLOG_COMP_REC_INSERT);
            {
                ulint    offset;
                ulint    end_seg_len;
                ulint    origin_offset        = 0; /* remove warning */
                ulint    mismatch_index        = 0; /* remove warning */
                ulint    info_and_status_bits = 0; /* remove warning */
            
                if (UNIV_UNLIKELY(end_ptr < ptr + 2)) {
                
                    return(NULL);
                }
                offset = mach_read_from_2(ptr);
                ptr += 2;
                if (offset >= UNIV_PAGE_SIZE) {
                    return(NULL);
                }
                end_seg_len = mach_parse_compressed((const byte**)&ptr, (const byte*)end_ptr);
                if (ptr == NULL) {
                    return(NULL);
                }
                if (end_seg_len >= UNIV_PAGE_SIZE << 1) {
                    return(NULL);
                }
                if (end_seg_len & 0x1UL)
                {
                    if (end_ptr < ptr + 1) {
                        return(NULL);
                    }
                
                    info_and_status_bits = mach_read_from_1(ptr);
                    ptr++;
                    
                    origin_offset = mach_parse_compressed((const byte**)&ptr, (const byte*)end_ptr);
                
                    if (ptr == NULL) {
                
                        return(NULL);
                    }
                
                    mismatch_index = mach_parse_compressed((const byte**)&ptr, (const byte*)end_ptr);
                
                    if (ptr == NULL) {
                
                        return(NULL);
                    }
                }
                fprintf(fp_dump_logrec,"prev_rec_offset:%ld    diff_len:%ld  info_bits:%ld extra_size:%ld mismatch_idx:%ld \n"
                   ,offset,(end_seg_len-1)/2,info_and_status_bits,origin_offset,mismatch_index);
                if (end_ptr < ptr + (end_seg_len >> 1)) 
                {
                
                    return(NULL);
                }
                byte *recbgn=ptr;
                ptr+=(end_seg_len >> 1);
                byte *recend=ptr;
                for(int i=0;recbgn<recend;)
                {
                  fprintf(fp_dump_logrec," %02x",recbgn[0]);
                  i++;
                  if(0==i%32)fprintf(fp_dump_logrec,"\n");
                  recbgn++;
                }
                fprintf(fp_dump_logrec,"\n");
            }
            //page_cur_parse_insert_rec
          }
          break;
     case MLOG_REC_CLUST_DELETE_MARK: case MLOG_COMP_REC_CLUST_DELETE_MARK:  
       if (NULL != (ptr = mlog_parse_index(
                ptr, end_ptr,
                type == MLOG_COMP_REC_CLUST_DELETE_MARK,
                &index))) 
          {
            btr_cur_print_index(fp_dump_logrec,index,type == MLOG_COMP_REC_CLUST_DELETE_MARK);
            /*ptr = btr_cur_parse_del_mark_set_clust_rec(
                    ptr, end_ptr, page, page_zip, index);*/
            {
               ulint        flags;
               ulint        val;
               ulint        pos;
               ulint        offset;
               trx_id_t    trx_id;
               roll_ptr_t    roll_ptr;
               if (end_ptr < ptr + 2) {
               
                   return(NULL);
               }
               flags = mach_read_from_1(ptr);
               ptr++;
               val = mach_read_from_1(ptr);
               ptr++;
               ptr = row_upd_parse_sys_vals(ptr, end_ptr, &pos, &trx_id, &roll_ptr);
               if (ptr == NULL) {
                   return(NULL);
               }
               if (end_ptr < ptr + 2) {
                   return(NULL);
               }
               offset = mach_read_from_2(ptr);
               ptr += 2;
               fprintf(fp_dump_logrec,"org_flags:%ld   new_flags:%ld  trx_id_pos:%ld trx_id:%ld roll_ptr:%ld page_offset:%ld \n",
                          flags,val,pos,trx_id,roll_ptr,offset);
             }
       }
       break;
       
     case MLOG_COMP_REC_SEC_DELETE_MARK:
       ptr = mlog_parse_index(ptr, end_ptr, TRUE, &index);
       btr_cur_print_index(fp_dump_logrec,index,type == MLOG_COMP_REC_SEC_DELETE_MARK);
       if (!ptr) {
         break;
       }
       /* Fall through */
     case MLOG_REC_SEC_DELETE_MARK:
       /*ptr = btr_cur_parse_del_mark_set_sec_rec(ptr, end_ptr,
                  page, page_zip);*/
       {
         ulint    val;
         ulint    offset;
         if (end_ptr < ptr + 3) {
             return(NULL);
         }
         val = mach_read_from_1(ptr);
         ptr++;
         offset = mach_read_from_2(ptr);
         ptr += 2;
         fprintf(fp_dump_logrec,"new_flags:%ld  page_offset:%ld \n",val,offset); 
       }
       break;
       
     case MLOG_REC_UPDATE_IN_PLACE: case MLOG_COMP_REC_UPDATE_IN_PLACE:
       if (NULL != (ptr = mlog_parse_index(
                ptr, end_ptr,
                type == MLOG_COMP_REC_UPDATE_IN_PLACE,
                &index))) 
       {
         btr_cur_print_index(fp_dump_logrec,index,type == MLOG_COMP_REC_UPDATE_IN_PLACE);
         ptr = btr_cur_parse_log_update_in_place(fp_dump_logrec,ptr, end_ptr);
       }
       break;
     case MLOG_LIST_END_DELETE: case MLOG_COMP_LIST_END_DELETE:
     case MLOG_LIST_START_DELETE: case MLOG_COMP_LIST_START_DELETE:
       if (NULL != (ptr = mlog_parse_index(
                ptr, end_ptr,
                type == MLOG_COMP_LIST_END_DELETE
                || type == MLOG_COMP_LIST_START_DELETE,
                &index))) {
             btr_cur_print_index(fp_dump_logrec,index,type == MLOG_COMP_LIST_END_DELETE||type==MLOG_COMP_LIST_START_DELETE);
             fprintf(fp_dump_logrec,"page_offset:%ld \n",mach_read_from_2(ptr));
             ptr+=2;
       }
       break;
     case MLOG_LIST_END_COPY_CREATED: case MLOG_COMP_LIST_END_COPY_CREATED:
       if (NULL != (ptr = mlog_parse_index(
                ptr, end_ptr,
                type == MLOG_COMP_LIST_END_COPY_CREATED,
                &index))) {
            btr_cur_print_index(fp_dump_logrec,index,type == MLOG_COMP_LIST_END_COPY_CREATED);
            byte*        rec_end;
            byte*        rec;
            ulint        log_data_len;
            
            if (ptr + 4 > end_ptr) {
            
                return(NULL);
            }
            
            log_data_len = mach_read_from_4(ptr);
            ptr += 4;
            rec=ptr;
            ptr+=log_data_len;
            rec_end = ptr;
            if (rec_end > end_ptr) {
                return(NULL);
            }
            fprintf(fp_dump_logrec,"undo_data_len:%ld undo_data: \n",log_data_len);
            for(ulint i=0;rec<rec_end;)
            {
              fprintf(fp_dump_logrec," %02x",rec[0]);
              rec++;
              if(0==++i%32)fprintf(fp_dump_logrec,"\n");
            }
            fprintf(fp_dump_logrec,"\n");
       }
       break;
     case MLOG_PAGE_REORGANIZE:
     case MLOG_COMP_PAGE_REORGANIZE:
     case MLOG_ZIP_PAGE_REORGANIZE:
       if (NULL != (ptr = mlog_parse_index(
                ptr, end_ptr,
                type != MLOG_PAGE_REORGANIZE,
                &index))) 
       {
         btr_cur_print_index(fp_dump_logrec,index,type == MLOG_COMP_PAGE_REORGANIZE);
         if(type == MLOG_ZIP_PAGE_REORGANIZE)
         {
            if (ptr == end_ptr) {
              return(NULL);
             }
          fprintf(fp_dump_logrec,"level:%ld \n",mach_read_from_1(ptr));
          ++ptr;
         }
       }
       break;
     case MLOG_PAGE_CREATE: case MLOG_COMP_PAGE_CREATE:
       /* Allow anything in page_type when creating a page. */
       break;
     case MLOG_PAGE_CREATE_RTREE: case MLOG_COMP_PAGE_CREATE_RTREE:
       break;
     case MLOG_UNDO_INSERT:
     {
         ulint    len;
         byte*    rec;
         byte*    recend;
         if (end_ptr < ptr + 2) {
             return(NULL);
         }
         len = mach_read_from_2(ptr);
         ptr += 2;
         if (end_ptr < ptr + len) {
             return(NULL);
         }
         rec=ptr;
         ptr+=len;
         recend=ptr;
         fprintf(fp_dump_logrec,"undo_len:%ld undo_data: \n",len);
         for(ulint i=0;rec<recend;)
         {
           fprintf(fp_dump_logrec," %02x",rec[0]);
           rec++;
           if(0==++i%32)fprintf(fp_dump_logrec,"\n");
         }
         fprintf(fp_dump_logrec,"\n");

     }
      break;
     case MLOG_UNDO_ERASE_END:
       break;
     case MLOG_UNDO_INIT:
     {
         ulint   type;
         type = mach_parse_compressed((const byte**)&ptr, (const byte*)end_ptr);
         if (ptr == NULL) {
             return(NULL);
         }
         fprintf(fp_dump_logrec,"undo_type:%ld (1-UNDO_INSERT 2-UNDO_UPDATE)\n",type);
     }
       break;
     case MLOG_UNDO_HDR_DISCARD:
       break;
     case MLOG_UNDO_HDR_CREATE:
     case MLOG_UNDO_HDR_REUSE:
      {
        trx_id_t    trx_id = mach_u64_parse_compressed((const byte**)&ptr, (const byte*)end_ptr);
        fprintf(fp_dump_logrec,"trx_id:%ld \n",trx_id);
      }
       break;
     case MLOG_REC_MIN_MARK: case MLOG_COMP_REC_MIN_MARK:
     {
       fprintf(fp_dump_logrec,"page_offset:%ld \n",mach_read_from_2(ptr));
       ptr+=2;
     }
       break;
     case MLOG_REC_DELETE: case MLOG_COMP_REC_DELETE:
       if (NULL != (ptr = mlog_parse_index(
                ptr, end_ptr,
                type == MLOG_COMP_REC_DELETE,
                &index))) 
       {
         btr_cur_print_index(fp_dump_logrec,index,type == MLOG_COMP_REC_DELETE);
         ulint        offset;
         if (end_ptr < ptr + 2) {
             return(NULL);
         }
         /* Read the cursor rec offset as a 2-byte ulint */
         offset = mach_read_from_2(ptr);
         fprintf(fp_dump_logrec,"page_offset:%ld \n",offset);
         ptr += 2;
       }
       break;
     case MLOG_IBUF_BITMAP_INIT:
       break;
     case MLOG_INIT_FILE_PAGE:
     case MLOG_INIT_FILE_PAGE2:
       break;
     case MLOG_WRITE_STRING:
     {
         ulint    offset;
         ulint    len;
         if (end_ptr < ptr + 4) {
             return(NULL);
         }
         offset = mach_read_from_2(ptr);
         ptr += 2;
         len = mach_read_from_2(ptr);
         ptr += 2;
         if (end_ptr < ptr + len) {
             return(NULL);
         }
         byte *rec=ptr;
         ptr+=len;
         byte *rec_end=ptr;
         fprintf(fp_dump_logrec,"page_offset:%ld data_len:%ld data:\n",offset,len);
         for(ulint i=0;rec<rec_end;)
         {
           fprintf(fp_dump_logrec," %02x",rec[0]);
           rec++;
           if(0==++i%32)fprintf(fp_dump_logrec,"\n");
         }
         fprintf(fp_dump_logrec,"\n");
     }
       break;
     case MLOG_ZIP_WRITE_NODE_PTR:
       ptr = page_zip_parse_write_node_ptr(ptr, end_ptr,
                   page, page_zip);
       break;
     case MLOG_ZIP_WRITE_BLOB_PTR:
       ptr = page_zip_parse_write_blob_ptr(ptr, end_ptr,
                   page, page_zip);
       break;
     case MLOG_ZIP_WRITE_HEADER:
       ptr = page_zip_parse_write_header(ptr, end_ptr,
                 page, page_zip);
       break;
     case MLOG_ZIP_PAGE_COMPRESS:
       /* Allow anything in page_type when creating a page. */
       ptr = page_zip_parse_compress(ptr, end_ptr,
                   page, page_zip);
       break;
     case MLOG_ZIP_PAGE_COMPRESS_NO_DATA:
       if (NULL != (ptr = mlog_parse_index(
           ptr, end_ptr, TRUE, &index))) 
       {
         ptr = page_zip_parse_compress_no_data(
           ptr, end_ptr, page, page_zip, index);
       }
       break;
     default:
       ptr = NULL;
  }
  return(ptr);
}
/****************************************
 btr_cur_parse_log_rec
 ***************************************/
static ulint 
btr_cur_parse_log_rec(
FILE* fp_dump_logrec,
byte* ptr,
byte* end_ptr,
ulint*    space,
ulint*    page_no,
mlog_id_t*    type
)
{
  byte* new_ptr=NULL;  
  bool   single_rec=true;
  if (ptr == end_ptr) {
    return(0);
  }
  switch (*ptr) {
      case MLOG_CHECKPOINT:
#ifdef UNIV_LOG_LSN_DEBUG
      case MLOG_LSN:
#endif /* UNIV_LOG_LSN_DEBUG */
      case MLOG_DUMMY_RECORD:
          single_rec = true;
          break;
      default:
          single_rec = !!(*ptr & MLOG_SINGLE_REC_FLAG);
  }

  switch (*ptr) {
#ifdef UNIV_LOG_LSN_DEBUG
  case MLOG_LSN | MLOG_SINGLE_REC_FLAG:
  case MLOG_LSN:
    new_ptr = mlog_parse_initial_log_record(
      ptr, end_ptr, type, space, page_no);
    *type = MLOG_LSN;
    fprintf(fp_dump_logrec, "redolog##: single_rec=%d   type=%s    space=%ld    page_no=%ld   len=%ld \n",
                              single_rec,get_mlog_string(*type), *space,*page_no, new_ptr-ptr);
    return(new_ptr - ptr);
#endif /* UNIV_LOG_LSN_DEBUG */
  case MLOG_MULTI_REC_END:
  case MLOG_DUMMY_RECORD:
    *type = static_cast<mlog_id_t>(*ptr);
    fprintf(fp_dump_logrec, "redolog##: single_rec=%d   type=%s      len=1\n",
                              single_rec,btr_get_mlog_string(*type));
    return(1);
  case MLOG_CHECKPOINT:
    if (end_ptr < ptr + SIZE_OF_MLOG_CHECKPOINT) {
      return(0);
    }
    *type = static_cast<mlog_id_t>(*ptr);
    fprintf(fp_dump_logrec, "redolog##: single_rec=%d   type=%s    checkpoint_lsn=%ld  len=9 \n",
                              single_rec,btr_get_mlog_string(*type),mach_read_from_8(ptr+1));
    return(SIZE_OF_MLOG_CHECKPOINT);
  case MLOG_MULTI_REC_END | MLOG_SINGLE_REC_FLAG:
  case MLOG_DUMMY_RECORD | MLOG_SINGLE_REC_FLAG:
  case MLOG_CHECKPOINT | MLOG_SINGLE_REC_FLAG:
    recv_sys->found_corrupt_log = true;
    return(0);
  }
  
  new_ptr = mlog_parse_initial_log_record(ptr, end_ptr, type, space,
            page_no);
   fprintf(fp_dump_logrec, "redolog##: single_rec=%d   type=%s  space_id=%ld  page_no=%ld \n",
                              single_rec,btr_get_mlog_string(*type),*space,*page_no);
  if (UNIV_UNLIKELY(!new_ptr)) {

    return(0);
  }
   new_ptr=btr_cur_parse_log_rec_body(fp_dump_logrec,*type,new_ptr,end_ptr,*space,*page_no,NULL,NULL);
  if (UNIV_UNLIKELY(new_ptr == NULL)) {

    return(0);
  }

  return(new_ptr - ptr);
  
}
/****************************************************
  btr_cur_dump_log_recs
 ****************************************************/
static
void btr_cur_dump_all_log_recs(
FILE* fp_dmp_redo_block,
 byte* bgnptr,
uint recv_len)
{
  byte*        ptr;    
  byte*        end_ptr;    
  ulint        len;      
  mlog_id_t    type;    
  ulint        space;    
  ulint        page_no;
  ptr=bgnptr;
  end_ptr=bgnptr+recv_len;
  if(ptr>=end_ptr)
  {
    return;
  }
  while(ptr<end_ptr)
  {
    len = btr_cur_parse_log_rec(fp_dmp_redo_block, ptr, end_ptr, &space,
                 &page_no, &type);
    /*fprintf(fp_dmp_redo_block,"single_rec=%d  type:%-35.35s  space_id=%-10ld  page_no=%-10ld len=%ld \n",
           single_rec,btr_get_mlog_string(type),space,page_no,len);*/
    if (len == 0) 
    {
        return ;
    }
    ptr+=len;
  }

}
/******************************************************
 btr_cur_dump_redo_block_body
 *****************************************************/
static
void btr_cur_dump_rangelsn_recs_body(
  FILE* fp_dmp_redo_block,
  const byte*  log_block,
  ulint  lsn_offset
)
{
  ulint offset=0;
  uint i=0;
  byte  recv_buf[32*OS_FILE_LOG_BLOCK_SIZE]={0};
  ulint recv_len=0;
  uint firstrec_offset=0;
  uint data_len=0;
  const byte* rec=log_block;
  while(offset<=lsn_offset)
  {
    data_len=mach_read_from_2(rec+LOG_BLOCK_HDR_DATA_LEN);
    firstrec_offset=mach_read_from_2(rec+LOG_BLOCK_FIRST_REC_GROUP);
    if(i==0)
    {/*第一个logblock*/
      if(data_len==OS_FILE_LOG_BLOCK_SIZE)
      {
        data_len-=LOG_BLOCK_TRL_SIZE;
      }
      data_len-=firstrec_offset;
      ut_memcpy(recv_buf+recv_len,log_block+firstrec_offset,data_len);
    }
    else
    {
       if(offset==lsn_offset)
       {/*最后一个logblock*/
         data_len=firstrec_offset-LOG_BLOCK_HDR_SIZE;
       }
       else
       {
         if(data_len==OS_FILE_LOG_BLOCK_SIZE)
         {
           data_len-=LOG_BLOCK_TRL_SIZE;
         }   
         data_len-=LOG_BLOCK_HDR_SIZE;
       }
       ut_memcpy(recv_buf+recv_len,log_block+12,data_len);
    }
    recv_len+=data_len;
    i++;
    offset+=OS_FILE_LOG_BLOCK_SIZE;
    log_block+=OS_FILE_LOG_BLOCK_SIZE;
    rec=log_block;
  }
  if(i==1)
  {
    fprintf(fp_dmp_redo_block,"one logblock have no end record.\n");
    return;
  }
  fprintf(fp_dmp_redo_block,"------------------------\n"
                             "start print log records:\n");
  btr_cur_dump_all_log_recs(fp_dmp_redo_block,recv_buf,recv_len);
  return;
}
/******************************************************
 btr_cur_dump_redo_block
 *****************************************************/
void btr_cur_dump_rangelsn_recs(
   FILE* fp_dmp_redo_block,
   ulint lsn1,
   ulint lsn2)
{
    fprintf(fp_dmp_redo_block,"--------------------------------\n");
   lsn_t start_lsn=(lsn_t)lsn1;
   start_lsn=ut_uint64_align_down(start_lsn,OS_FILE_LOG_BLOCK_SIZE);
   if(lsn1>lsn2)
   {
     fprintf(fp_dmp_redo_block,"start_lsn > end_lsn\n");
     return;
   }
   lsn_t end_lsn=(lsn_t)lsn2;
   end_lsn=ut_uint64_align_down(end_lsn,OS_FILE_LOG_BLOCK_SIZE);
   fprintf(fp_dmp_redo_block,"start_lsn:%ld    align_down :%ld\n"
                             "end_lsn  :%ld    align_down :%ld\n"
                              ,lsn1,start_lsn
                              ,lsn2,end_lsn);
   if(end_lsn==start_lsn)
   {
      end_lsn+=OS_FILE_LOG_BLOCK_SIZE;
   }
   else if(end_lsn-start_lsn>32*OS_FILE_LOG_BLOCK_SIZE)
   {
      fprintf(fp_dmp_redo_block,"end_lsn-start_lsn>32*512 max interval==32*512\n");
      end_lsn+=32*OS_FILE_LOG_BLOCK_SIZE;
   }
   log_mutex_enter();
   log_group_t*    group;
   group = UT_LIST_GET_FIRST(log_sys->log_groups);
   byte   buf[32*OS_FILE_LOG_BLOCK_SIZE]={0};
   log_group_read_log_seg(buf, group, start_lsn, end_lsn+OS_FILE_LOG_BLOCK_SIZE);
   log_mutex_exit();
   const byte*    log_block    = buf;
   btr_cur_dump_rangelsn_recs_body(fp_dmp_redo_block,log_block,end_lsn-start_lsn);
   fprintf(fp_dmp_redo_block,"--------------------------------\n");
   return;
}
/****************************************************
  btr_cur_dump_log_recs
 ****************************************************/
static
void btr_cur_dump_log_recs_type(
FILE* fp_dmp_redo_block,
 byte* bgnptr,
uint recv_len)
{
  byte*        ptr;    
  byte*        end_ptr;    
  ulint        len;      
  mlog_id_t    type;    
  ulint        space;    
  ulint        page_no;
  byte*        body;
  bool        single_rec;
  ptr=bgnptr;
  end_ptr=bgnptr+recv_len;
  if(ptr>=end_ptr)
  {
    return;
  }
  while(ptr<end_ptr)
  {
    switch (*ptr) {
    case MLOG_CHECKPOINT:
    #ifdef UNIV_LOG_LSN_DEBUG
    case MLOG_LSN:
    #endif /* UNIV_LOG_LSN_DEBUG */
    case MLOG_DUMMY_RECORD:
        single_rec = true;
        break;
    default:
        single_rec = !!(*ptr & MLOG_SINGLE_REC_FLAG);
    }
    len = recv_parse_log_rec(&type, ptr, end_ptr, &space,
                 &page_no, true, &body);
    fprintf(fp_dmp_redo_block,"single_rec=%d  type:%-35.35s  space_id=%-10ld  page_no=%-10ld len=%ld \n",
           single_rec,btr_get_mlog_string(type),space,page_no,len);
    if (len == 0) 
    {
        return ;
    }
    ptr+=len;
  }

}
/******************************************************
 btr_cur_dump_redo_block_body
 *****************************************************/
static
void btr_cur_dump_redo_block_body(
  FILE* fp_dmp_redo_block,
  const byte*  log_block,
  ulint  lsn_offset
)
{
  ulint offset=0;
  uint i=0;
  byte  recv_buf[32*OS_FILE_LOG_BLOCK_SIZE]={0};
  ulint recv_len=0;
  uint firstrec_offset=0;
  uint data_len=0;
  const byte* rec=log_block;
  while(offset<=lsn_offset)
  {
    fprintf(fp_dmp_redo_block,"-------------\n"
                              " %u th redo block:\n"
                              ,i);
    fprintf(fp_dmp_redo_block, "LOG_BLOCK_HDR_NO: %02x %02x %02x %02x (%ld)\n",
                                rec[0],rec[1],rec[2],rec[3], (~LOG_BLOCK_FLUSH_BIT_MASK & mach_read_from_4(rec)));
     rec+=4;
    fprintf(fp_dmp_redo_block, "LOG_BLOCK_HDR_DATA_LEN: %02x %02x (%ld)\n",
                                rec[0],rec[1], mach_read_from_2(rec));
     data_len=mach_read_from_2(rec);
     rec+=2;
    fprintf(fp_dmp_redo_block, "LOG_BLOCK_FIRST_REC_GROUP:%02x %02x (%ld)\n",
                                rec[0],rec[1], mach_read_from_2(rec));
     firstrec_offset=mach_read_from_2(rec);
     rec+=2;
    fprintf(fp_dmp_redo_block, "LOG_BLOCK_CHECKPOINT_NO: %02x %02x %02x %02x (%ld)\n",
                                rec[0],rec[1],rec[2],rec[3], mach_read_from_4(rec));
    if(firstrec_offset>0)
    {
       rec=log_block+firstrec_offset;
       mlog_id_t type=(enum mlog_id_t)(*rec & 0x7F);
       fprintf(fp_dmp_redo_block, "firstrec_offset:%u  type:%s\n"
                                  ,firstrec_offset,btr_get_mlog_string(type));
    }
    else
    {
       fprintf(fp_dmp_redo_block, "LOG_BLOCK_FIRST_REC_GROUP is 0 \n");
    }
    if(i==0)
    {/*第一个logblock*/
      if(data_len==OS_FILE_LOG_BLOCK_SIZE)
      {
        data_len-=LOG_BLOCK_TRL_SIZE;
      }
      data_len-=firstrec_offset;
      ut_memcpy(recv_buf+recv_len,log_block+firstrec_offset,data_len);
    }
    else
    {
       if(offset==lsn_offset)
       {/*最后一个logblock*/
         data_len=firstrec_offset-LOG_BLOCK_HDR_SIZE;
       }
       else
       {
         if(data_len==OS_FILE_LOG_BLOCK_SIZE)
         {
           data_len-=LOG_BLOCK_TRL_SIZE;
         }   
         data_len-=LOG_BLOCK_HDR_SIZE;
       }
       ut_memcpy(recv_buf+recv_len,log_block+12,data_len);
    }
    recv_len+=data_len;
    i++;
    offset+=OS_FILE_LOG_BLOCK_SIZE;
    log_block+=OS_FILE_LOG_BLOCK_SIZE;
    rec=log_block;
  }
  if(i==1)
  {
    fprintf(fp_dmp_redo_block,"one logblock have no end record.\n");
    return;
  }
  fprintf(fp_dmp_redo_block,"------------------------\n"
                             "start print log records:\n");
  btr_cur_dump_log_recs_type(fp_dmp_redo_block,recv_buf,recv_len);
  return;
}
/******************************************************
 btr_cur_dump_redo_block
 *****************************************************/
void btr_cur_dump_redo_block(
   FILE* fp_dmp_redo_block,
   ulint lsn1,
   ulint lsn2)
{
    fprintf(fp_dmp_redo_block,"--------------------------------\n");
   lsn_t start_lsn=(lsn_t)lsn1;
   start_lsn=ut_uint64_align_down(start_lsn,OS_FILE_LOG_BLOCK_SIZE);
   if(lsn1>lsn2)
   {
     fprintf(fp_dmp_redo_block,"start_lsn > end_lsn\n");
     return;
   }
   lsn_t end_lsn=(lsn_t)lsn2;
   end_lsn=ut_uint64_align_down(end_lsn,OS_FILE_LOG_BLOCK_SIZE);
   fprintf(fp_dmp_redo_block,"start_lsn:%ld    align_down :%ld\n"
                             "end_lsn  :%ld    align_down :%ld\n"
                              ,lsn1,start_lsn
                              ,lsn2,end_lsn);
   if(end_lsn==start_lsn)
   {
      end_lsn+=OS_FILE_LOG_BLOCK_SIZE;
   }
   else if(end_lsn-start_lsn>32*OS_FILE_LOG_BLOCK_SIZE)
   {
      fprintf(fp_dmp_redo_block,"end_lsn-start_lsn>32*512 max interval==32*512\n");
      end_lsn+=32*OS_FILE_LOG_BLOCK_SIZE;
   }
   log_mutex_enter();
   log_group_t*    group;
   group = UT_LIST_GET_FIRST(log_sys->log_groups);
   byte   buf[32*OS_FILE_LOG_BLOCK_SIZE]={0};
   log_group_read_log_seg(buf, group, start_lsn, end_lsn+OS_FILE_LOG_BLOCK_SIZE);
   log_mutex_exit();
   const byte*    log_block    = buf;
   btr_cur_dump_redo_block_body(fp_dmp_redo_block,log_block,end_lsn-start_lsn);
   fprintf(fp_dmp_redo_block,"--------------------------------\n");
   return;
}
/*dump double write buffer header*/
static
void
btr_cur_dump_doublewrite_bufhdr
(
FILE*         fp_dmp_doublewrite,
byte*   page
)
{
    byte* doublewrite=page+TRX_SYS_DOUBLEWRITE;
    byte* rec=doublewrite;
    fprintf(fp_dmp_doublewrite,"--------------------------------\n");
    fprintf(fp_dmp_doublewrite,"DOUBLEWRITE BUFFER HEADER:\n");
    rec=doublewrite+TRX_SYS_DOUBLEWRITE_FSEG;
    fprintf(fp_dmp_doublewrite,"TRX_SYS_DOUBLEWRITE_FSEG:\n"
                                "  SPACE_ID:%02x %02x %02x %2x (%ld)\n"
                                "  PAGE_NO: %02x %02x %02x %2x (%ld)\n"
                                "  OFFSET:  %02x %02x (%ld)\n",
                                rec[0],rec[1],rec[2],rec[3],mach_read_from_4(rec),
                                rec[4],rec[5],rec[6],rec[7],mach_read_from_4(rec+4),
                                rec[8],rec[9],mach_read_from_2(rec+8));
    rec=doublewrite+TRX_SYS_DOUBLEWRITE_MAGIC;
    fprintf(fp_dmp_doublewrite,"TRX_SYS_DOUBLEWRITE_MAGIC:%02x %02x %02x %2x (%ld)\n",
                                 rec[0],rec[1],rec[2],rec[3],mach_read_from_4(rec));
    rec=doublewrite+TRX_SYS_DOUBLEWRITE_BLOCK1;
    fprintf(fp_dmp_doublewrite,"TRX_SYS_DOUBLEWRITE_BLOCK1:%02x %02x %02x %2x (%ld)\n",
                                 rec[0],rec[1],rec[2],rec[3],mach_read_from_4(rec));
    rec=doublewrite+TRX_SYS_DOUBLEWRITE_BLOCK2;
    fprintf(fp_dmp_doublewrite,"TRX_SYS_DOUBLEWRITE_BLOCK2:%02x %02x %02x %2x (%ld)\n",
                                 rec[0],rec[1],rec[2],rec[3],mach_read_from_4(rec));
    rec=doublewrite+FSEG_HEADER_SIZE+TRX_SYS_DOUBLEWRITE_REPEAT;
    fprintf(fp_dmp_doublewrite,"TRX_SYS_DOUBLEWRITE_REPEAT:\n");
    fprintf(fp_dmp_doublewrite,"   TRX_SYS_DOUBLEWRITE_MAGIC:%02x %02x %02x %2x (%ld)\n",
                                 rec[0],rec[1],rec[2],rec[3],mach_read_from_4(rec));
    rec=rec+4;
    fprintf(fp_dmp_doublewrite,"   TRX_SYS_DOUBLEWRITE_BLOCK1:%02x %02x %02x %2x (%ld)\n",
                                 rec[0],rec[1],rec[2],rec[3],mach_read_from_4(rec));
    rec=rec+4;
    fprintf(fp_dmp_doublewrite,"   TRX_SYS_DOUBLEWRITE_BLOCK2:%02x %02x %02x %2x (%ld)\n",
                                 rec[0],rec[1],rec[2],rec[3],mach_read_from_4(rec));
    rec=doublewrite+TRX_SYS_DOUBLEWRITE_SPACE_ID_STORED;
    fprintf(fp_dmp_doublewrite,"TRX_SYS_DOUBLEWRITE_SPACE_ID_STORED:%02x %02x %02x %2x (%ld)\n",
                                 rec[0],rec[1],rec[2],rec[3],mach_read_from_4(rec)); 
    return;
}
/*btr_cur_get_pagetype_string*/
static 
const char* btr_cur_get_pagetype_string(uint pagetype)
{
 switch(pagetype)
 {
 case FIL_PAGE_TYPE_ALLOCATED:   return "FIL_PAGE_TYPE_ALLOCATED";
 case FIL_PAGE_TYPE_BLOB:        return "FIL_PAGE_TYPE_BLOB";
 case FIL_PAGE_TYPE_FSP_HDR:     return "FIL_PAGE_TYPE_FSP_HDR";
 case FIL_PAGE_TYPE_UNKNOWN:     return "FIL_PAGE_TYPE_UNKNOWN";
 case FIL_PAGE_TYPE_SYS:         return "FIL_PAGE_TYPE_SYS";
 case FIL_PAGE_TYPE_TRX_SYS:     return "FIL_PAGE_TYPE_TRX_SYS";
 case FIL_PAGE_TYPE_XDES:        return "FIL_PAGE_TYPE_XDES";
 case FIL_PAGE_TYPE_ZBLOB:       return "FIL_PAGE_TYPE_ZBLOB";
 case FIL_PAGE_TYPE_ZBLOB2:      return "FIL_PAGE_TYPE_ZBLOB2";
 case FIL_PAGE_INDEX:            return "FIL_PAGE_INDEX";
 case FIL_PAGE_INODE:            return "FIL_PAGE_INODE";
 case FIL_PAGE_UNDO_LOG:         return "FIL_PAGE_UNDO_LOG";
 case FIL_PAGE_COMPRESSED:       return "FIL_PAGE_COMPRESSED";
 case FIL_PAGE_COMPRESSED_AND_ENCRYPTED: return "FIL_PAGE_COMPRESSED_AND_ENCRYPTED";
 case FIL_PAGE_ENCRYPTED:        return "FIL_PAGE_ENCRYPTED";
 case FIL_PAGE_ENCRYPTED_RTREE:  return "FIL_PAGE_ENCRYPTED_RTREE";
 case FIL_PAGE_IBUF_BITMAP:      return "FIL_PAGE_IBUF_BITMAP";
 case FIL_PAGE_IBUF_FREE_LIST:   return "FIL_PAGE_IBUF_FREE_LIST";
 default: return "FIL_PAGE_TYPE_UNKNOWN";
 }
}
/**dump double write blocks**/
static void
btr_cur_dump_doublewrite_blocks(
FILE * fp_dmp_doublewrite,
ulint bgn_pageno,
uint  idx
)
{
  fprintf(fp_dmp_doublewrite,"-----------------\n"
                             "block%u: space_id=0 page_no:%ld ~ %ld \n",idx,bgn_pageno,bgn_pageno+FSP_EXTENT_SIZE-1);
   bool found;
   const page_size_t&    page_size
       = fil_space_get_page_size(TRX_SYS_SPACE, &found);
  if(false==found)
  {
     fprintf(fp_dmp_doublewrite,"space_id=%ld is not exist\n",TRX_SYS_SPACE);
     return;
  }
  for(uint i=0;i<FSP_EXTENT_SIZE;i++)
  {
      buf_block_t*  block;
      mtr_t mtr;
      byte*  page;
      page_id_t  page_id(TRX_SYS_SPACE,bgn_pageno+i);
      mtr_start(&mtr);
      block = buf_page_get_gen(page_id, page_size, RW_NO_LATCH, NULL,
                       BUF_GET, __FILE__, __LINE__, &mtr);
      page = buf_block_get_frame(block);
      fprintf(fp_dmp_doublewrite,"page_no:%-5ld =[space_id:%-10ld   page_no:%-10ld  page_type:%-5ld (%s) ]\n",
                         bgn_pageno+i,
                         mach_read_from_4(page+FIL_PAGE_SPACE_ID),
                         mach_read_from_4(page+FIL_PAGE_OFFSET),
                         mach_read_from_2(page+FIL_PAGE_TYPE),
                         btr_cur_get_pagetype_string(mach_read_from_2(page+FIL_PAGE_TYPE))
              );
      mtr_commit(&mtr);
  }
} 
/**dump double write*/
void
btr_cur_dump_doublewrite
(  ulint         space_id,
ulint         page_no,
FILE*         fp_dmp_doublewrite)
{
   buf_block_t*    block=NULL;
   page_t*         page;
   mtr_t           mtr;

   page_id_t  page_id(space_id,page_no);
   bool            found;
   const page_size_t&    page_size
       = fil_space_get_page_size(page_id.space(), &found);
   if(false==found)
   {
      fprintf(fp_dmp_doublewrite,"space_id=%ld is not exist\n",space_id);
      return;
   }
   mtr_start(&mtr);
   block = buf_page_get_gen(page_id, page_size, RW_NO_LATCH, NULL,
                    BUF_GET, __FILE__, __LINE__, &mtr);
   if(NULL==block)
   {
     fprintf(fp_dmp_doublewrite,"space_id=%ld page_no=%ld is not exist\n",space_id,page_no);
     return;
   }
   page = buf_block_get_frame(block);
   if(FIL_PAGE_TYPE_TRX_SYS != fil_page_get_type(page))
   {
      fprintf(fp_dmp_doublewrite,"File page type is not Transaction system data node \n");
      return;
   }
   btr_cur_dump_doublewrite_bufhdr(fp_dmp_doublewrite,page);
   ulint bgn_pageno=mach_read_from_4(page+TRX_SYS_DOUBLEWRITE+TRX_SYS_DOUBLEWRITE_BLOCK1);
   btr_cur_dump_doublewrite_blocks(fp_dmp_doublewrite,bgn_pageno,1);
   bgn_pageno=mach_read_from_4(page+TRX_SYS_DOUBLEWRITE+TRX_SYS_DOUBLEWRITE_BLOCK2);
   btr_cur_dump_doublewrite_blocks(fp_dmp_doublewrite,bgn_pageno,2);
   fprintf(fp_dmp_doublewrite,"--------------------------------\n");
   return;    
}