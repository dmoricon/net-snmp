/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 1.18 $ of : mfd-interface.m2c,v $ 
 *
 * $Id$
 */
/*
 * *********************************************************************
 * *********************************************************************
 * *********************************************************************
 * ***                                                               ***
 * ***  NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE  ***
 * ***                                                               ***
 * ***                                                               ***
 * ***       THIS FILE DOES NOT CONTAIN ANY USER EDITABLE CODE.      ***
 * ***                                                               ***
 * ***                                                               ***
 * ***       THE GENERATED CODE IS INTERNAL IMPLEMENTATION, AND      ***
 * ***                                                               ***
 * ***                                                               ***
 * ***    IS SUBJECT TO CHANGE WITHOUT WARNING IN FUTURE RELEASES.   ***
 * ***                                                               ***
 * ***                                                               ***
 * *********************************************************************
 * *********************************************************************
 * *********************************************************************
 */

/*
 * standard Net-SNMP includes 
 */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

/*
 * include our parent header 
 */
#include "ifXTable.h"


#include <net-snmp/agent/table_mfd.h>
#include <net-snmp/library/container.h>

#include "ifXTable_interface.h"

/**********************************************************************
 **********************************************************************
 ***
 *** Table ifXTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * ifXTable is subid 1 of ifMIBObjects.
 * It's status is Current.
 * OID: .1.3.6.1.2.1.31.1.1, length: 9
 */
typedef struct ifXTable_interface_ctx_s {

    netsnmp_container *container;
    netsnmp_cache  *cache;      /* optional cache */

    ifXTable_registration_ptr user_ctx;

    netsnmp_table_registration_info tbl_info;

    netsnmp_baby_steps_access_methods access_multiplexer;

} ifXTable_interface_ctx;

static ifXTable_interface_ctx ifXTable_if_ctx;

static void     _ifXTable_container_init(ifXTable_interface_ctx * if_ctx);


static Netsnmp_Node_Handler _mfd_ifXTable_pre_request;
static Netsnmp_Node_Handler _mfd_ifXTable_post_request;
static Netsnmp_Node_Handler _mfd_ifXTable_object_lookup;
static Netsnmp_Node_Handler _mfd_ifXTable_get_values;
static Netsnmp_Node_Handler _mfd_ifXTable_check_objects;
static Netsnmp_Node_Handler _mfd_ifXTable_undo_setup;
static Netsnmp_Node_Handler _mfd_ifXTable_set_values;
static Netsnmp_Node_Handler _mfd_ifXTable_undo_values;
static Netsnmp_Node_Handler _mfd_ifXTable_commit;
static Netsnmp_Node_Handler _mfd_ifXTable_check_dependencies;

NETSNMP_STATIC_INLINE int _ifXTable_undo_column(ifXTable_rowreq_ctx *
                                                rowreq_ctx,
                                                netsnmp_variable_list *
                                                var, int column);


/**
 * @internal
 * Initialize the table ifXTable 
 *    (Define its contents and how it's structured)
 */
void
_ifXTable_initialize_interface(ifXTable_registration_ptr reg_ptr,
                               u_long flags)
{
    netsnmp_baby_steps_access_methods *access_multiplexer =
        &ifXTable_if_ctx.access_multiplexer;
    netsnmp_table_registration_info *tbl_info = &ifXTable_if_ctx.tbl_info;
    netsnmp_handler_registration *reginfo;
    netsnmp_mib_handler *handler;
    int             mfd_modes;

    DEBUGTRACE;


    /*************************************************
     *
     * save interface context for ifXTable
     */
    /*
     * Setting up the table's definition
     */
    netsnmp_table_helper_add_indexes(tbl_info, ASN_INTEGER,
                                               /** index: ifIndex */
                                     0);

    /*
     * Define the minimum and maximum accessible columns.  This
     * optimizes retrival. 
     */
    tbl_info->min_column = IFXTABLE_MIN_COL;
    tbl_info->max_column = IFXTABLE_MAX_COL;

    /*
     * save users context
     */
    ifXTable_if_ctx.user_ctx = reg_ptr;

    /*
     * set up the container
     */
    _ifXTable_container_init(&ifXTable_if_ctx);
    if (NULL == ifXTable_if_ctx.container) {
        snmp_log(LOG_ERR, "could not initialize container for ifXTable\n");
        return;
    }

    /*
     * access_multiplexer: REQUIRED wrapper for get request handling
     */
    access_multiplexer->object_lookup = _mfd_ifXTable_object_lookup;
    access_multiplexer->get_values = _mfd_ifXTable_get_values;


    /*
     * REQUIRED wrappers for set request handling
     */
    access_multiplexer->object_syntax_checks = _mfd_ifXTable_check_objects;
    access_multiplexer->undo_setup = _mfd_ifXTable_undo_setup;
    access_multiplexer->set_values = _mfd_ifXTable_set_values;
    access_multiplexer->undo_sets = _mfd_ifXTable_undo_values;

    /*
     * no wrappers yet
     */
    access_multiplexer->pre_request = _mfd_ifXTable_pre_request;
    access_multiplexer->post_request = _mfd_ifXTable_post_request;
    access_multiplexer->commit = _mfd_ifXTable_commit;

    /*
     * REQUIRED for tables with dependencies
     */
    access_multiplexer->consistency_checks =
        _mfd_ifXTable_check_dependencies;

    /*************************************************
     *
     * Create a registration, save our reg data, register table.
     */
    DEBUGMSGTL(("ifXTable:init_ifXTable",
                "Registering ifXTable as a mibs-for-dummies table.\n"));
    handler =
        netsnmp_baby_steps_access_multiplexer_get(access_multiplexer);
    reginfo =
        netsnmp_handler_registration_create("ifXTable", handler,
                                            ifXTable_oid,
                                            ifXTable_oid_size,
                                            HANDLER_CAN_BABY_STEP |
                                            HANDLER_CAN_RWRITE);
    if (NULL == reginfo) {
        snmp_log(LOG_ERR, "error registering tabel ifXTable\n");
        return;
    }
    reginfo->my_reg_void = &ifXTable_if_ctx;

    /*************************************************
     *
     * set up baby steps handler, create it and inject it
     */
    if (access_multiplexer->object_lookup)
        mfd_modes |= BABY_STEP_OBJECT_LOOKUP;
    if (access_multiplexer->set_values)
        mfd_modes |= BABY_STEP_SET_VALUES;
    if (access_multiplexer->irreversible_commit)
        mfd_modes |= BABY_STEP_IRREVERSIBLE_COMMIT;
    if (access_multiplexer->object_syntax_checks)
        mfd_modes |= BABY_STEP_CHECK_OBJECT;

    if (access_multiplexer->pre_request)
        mfd_modes |= BABY_STEP_PRE_REQUEST;
    if (access_multiplexer->post_request)
        mfd_modes |= BABY_STEP_POST_REQUEST;

    if (access_multiplexer->undo_setup)
        mfd_modes |= BABY_STEP_UNDO_SETUP;
    if (access_multiplexer->undo_cleanup)
        mfd_modes |= BABY_STEP_UNDO_CLEANUP;
    if (access_multiplexer->undo_sets)
        mfd_modes |= BABY_STEP_UNDO_SETS;

    if (access_multiplexer->row_creation)
        mfd_modes |= BABY_STEP_ROW_CREATE;
    if (access_multiplexer->consistency_checks)
        mfd_modes |= BABY_STEP_CHECK_CONSISTENCY;
    if (access_multiplexer->commit)
        mfd_modes |= BABY_STEP_COMMIT;
    if (access_multiplexer->undo_commit)
        mfd_modes |= BABY_STEP_UNDO_COMMIT;

    handler = netsnmp_baby_steps_handler_get(mfd_modes);
    netsnmp_inject_handler(reginfo, handler);

    /*************************************************
     *
     * inject container_table helper
     */
    handler =
        netsnmp_container_table_handler_get(tbl_info,
                                            ifXTable_if_ctx.container,
                                            TABLE_CONTAINER_KEY_NETSNMP_INDEX);
    netsnmp_inject_handler(reginfo, handler);

    /*************************************************
     *
     * inject row_merge helper with prefix rootoid_len + 2 (entry.col)
     */
    handler = netsnmp_get_row_merge_handler(reginfo->rootoid_len + 2);
    netsnmp_inject_handler(reginfo, handler);

    /*************************************************
     *
     * inject cache helper
     */
    if (NULL != ifXTable_if_ctx.cache) {
        handler = netsnmp_cache_handler_get(ifXTable_if_ctx.cache);
        netsnmp_inject_handler(reginfo, handler);
    }

    /*
     * register table
     */
    netsnmp_register_table(reginfo, tbl_info);
}

/**
 * @internal
 * convert the index component stored in the context to an oid
 */
int
ifXTable_index_to_oid(netsnmp_index * oid_idx,
                      ifXTable_mib_index * mib_idx)
{
    int             err = SNMP_ERR_NOERROR;

    /*
     * temp storage for parsing indexes
     */
    /** ifIndex(1)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/r/d/h */
    netsnmp_variable_list var_ifIndex;

    /*
     * set up varbinds
     */
    memset(&var_ifIndex, 0x00, sizeof(var_ifIndex));
    var_ifIndex.type = ASN_INTEGER;

    /*
     * chain temp index varbinds together
     */
    var_ifIndex.next_variable = NULL;


    DEBUGTRACE;

    /*
     * ifIndex(1)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/r/d/h 
     */
    snmp_set_var_value(&var_ifIndex, (u_char *) & mib_idx->ifIndex,
                       sizeof(mib_idx->ifIndex));


    err = build_oid_noalloc(oid_idx->oids, oid_idx->len, &oid_idx->len,
                            NULL, 0, &var_ifIndex);
    if (err)
        snmp_log(LOG_ERR, "error %d converting index to oid\n");

    /*
     * parsing may have allocated memory. free it.
     */
    snmp_reset_var_buffers(&var_ifIndex);

    return err;
}

/**
 * extract ifXTable indexes from a netsnmp_index
 *
 * @retval SNMP_ERR_NOERROR  : no error
 * @retval SNMP_ERR_GENERR   : error
 */
int
ifXTable_index_from_oid(netsnmp_index * oid_idx,
                        ifXTable_mib_index * mib_idx)
{
    int             err = SNMP_ERR_NOERROR;

    /*
     * temp storage for parsing indexes
     */
    /** ifIndex(1)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/r/d/h */
    netsnmp_variable_list var_ifIndex;

    /*
     * set up varbinds
     */
    memset(&var_ifIndex, 0x00, sizeof(var_ifIndex));
    var_ifIndex.type = ASN_INTEGER;

    /*
     * chain temp index varbinds together
     */
    var_ifIndex.next_variable = NULL;


    DEBUGTRACE;

    /*
     * parse the oid into the individual index components
     */
    err = parse_oid_indexes(oid_idx->oids, oid_idx->len, &var_ifIndex);
    if (err == SNMP_ERR_NOERROR) {
        /*
         * copy out values
         */
        mib_idx->ifIndex = *((long *) var_ifIndex.val.string);


    }

    /*
     * parsing may have allocated memory. free it.
     */
    snmp_reset_var_buffers(&var_ifIndex);

    return err;
}


/*
 *********************************************************************
 * @internal
 * allocate resources for a ifXTable_rowreq_ctx
 */
ifXTable_rowreq_ctx *
ifXTable_allocate_rowreq_ctx(void)
{
    ifXTable_rowreq_ctx *rowreq_ctx =
        SNMP_MALLOC_TYPEDEF(ifXTable_rowreq_ctx);

    DEBUGTRACE;

    if (NULL == rowreq_ctx) {
        snmp_log(LOG_ERR, "Couldn't allocate memory for a "
                 "ifXTable_rowreq_ctx.\n");
    }

    rowreq_ctx->oid_idx.oids = rowreq_ctx->oid_tmp;

    rowreq_ctx->ifXTable_data_list = NULL;
    rowreq_ctx->ifXTable_reg = ifXTable_if_ctx.user_ctx;


    return rowreq_ctx;
}

/*
 * @internal
 * release resources for a ifXTable_rowreq_ctx
 */
void
ifXTable_release_rowreq_ctx(ifXTable_rowreq_ctx * rowreq_ctx)
{
    DEBUGTRACE;

    netsnmp_assert(NULL != rowreq_ctx);



    /*
     * free index oid pointer
     */
    if (rowreq_ctx->oid_idx.oids != rowreq_ctx->oid_tmp)
        free(rowreq_ctx->oid_idx.oids);

    SNMP_FREE(rowreq_ctx);
}

/**
 * @internal
 * wrapper
 */
static int
_mfd_ifXTable_pre_request(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info *agtreq_info,
                          netsnmp_request_info *requests)
{
    return ifXTable_pre_request(ifXTable_if_ctx.user_ctx);
}

/**
 * @internal
 * wrapper
 */
static int
_mfd_ifXTable_post_request(netsnmp_mib_handler *handler,
                           netsnmp_handler_registration *reginfo,
                           netsnmp_agent_request_info *agtreq_info,
                           netsnmp_request_info *requests)
{
    return ifXTable_post_request(ifXTable_if_ctx.user_ctx);
}

/**
 * @internal
 * wrapper
 */
static int
_mfd_ifXTable_object_lookup(netsnmp_mib_handler *handler,
                            netsnmp_handler_registration *reginfo,
                            netsnmp_agent_request_info *agtreq_info,
                            netsnmp_request_info *requests)
{
    ifXTable_rowreq_ctx *rowreq_ctx =
        netsnmp_container_table_extract_context(requests);

    DEBUGTRACE;

    /*
     * get our context from mfd
     * ifXTable_interface_ctx *if_ctx =
     *             (ifXTable_interface_ctx *)reginfo->my_reg_void;
     */

    if (NULL == rowreq_ctx) {
        netsnmp_request_set_error_all(requests, SNMP_ERR_NOCREATION);
    }
    return MFD_SUCCESS;
}

/***********************************************************************
 *
 * GET processing
 *
 ***********************************************************************/
/*
 * @internal
 * Retrieve the value for a particular column
 */
NETSNMP_STATIC_INLINE int
_ifXTable_get_column(ifXTable_rowreq_ctx * rowreq_ctx,
                     netsnmp_variable_list * var, int column)
{
    int             rc = SNMPERR_SUCCESS;

    DEBUGTRACE;


    netsnmp_assert(NULL != rowreq_ctx);

    switch (column) {

        /*
         * ifName(1)/DisplayString/ASN_OCTET_STR/char(char)//L/A/w/e/R/d/H 
         */
    case COLUMN_IFNAME:
        var->type = ASN_OCTET_STR;
        rc = ifName_get(rowreq_ctx, (char **) &var->val.string,
                        &var->val_len);
        break;

        /*
         * ifInMulticastPkts(2)/COUNTER/ASN_COUNTER/u_long(u_long)//l/A/w/e/r/d/h 
         */
    case COLUMN_IFINMULTICASTPKTS:
        var->val_len = sizeof(u_long);
        var->type = ASN_COUNTER;
        rc = ifInMulticastPkts_get(rowreq_ctx, (u_long *) var->val.string);
        break;

        /*
         * ifInBroadcastPkts(3)/COUNTER/ASN_COUNTER/u_long(u_long)//l/A/w/e/r/d/h 
         */
    case COLUMN_IFINBROADCASTPKTS:
        var->val_len = sizeof(u_long);
        var->type = ASN_COUNTER;
        rc = ifInBroadcastPkts_get(rowreq_ctx, (u_long *) var->val.string);
        break;

        /*
         * ifOutMulticastPkts(4)/COUNTER/ASN_COUNTER/u_long(u_long)//l/A/w/e/r/d/h 
         */
    case COLUMN_IFOUTMULTICASTPKTS:
        var->val_len = sizeof(u_long);
        var->type = ASN_COUNTER;
        rc = ifOutMulticastPkts_get(rowreq_ctx,
                                    (u_long *) var->val.string);
        break;

        /*
         * ifOutBroadcastPkts(5)/COUNTER/ASN_COUNTER/u_long(u_long)//l/A/w/e/r/d/h 
         */
    case COLUMN_IFOUTBROADCASTPKTS:
        var->val_len = sizeof(u_long);
        var->type = ASN_COUNTER;
        rc = ifOutBroadcastPkts_get(rowreq_ctx,
                                    (u_long *) var->val.string);
        break;

        /*
         * ifHCInOctets(6)/COUNTER64/ASN_COUNTER64/U64(U64)//l/A/w/e/r/d/h 
         */
    case COLUMN_IFHCINOCTETS:
        var->val_len = sizeof(U64);
        var->type = ASN_COUNTER64;
        rc = ifHCInOctets_get(rowreq_ctx, (U64 *) var->val.string);
        break;

        /*
         * ifHCInUcastPkts(7)/COUNTER64/ASN_COUNTER64/U64(U64)//l/A/w/e/r/d/h 
         */
    case COLUMN_IFHCINUCASTPKTS:
        var->val_len = sizeof(U64);
        var->type = ASN_COUNTER64;
        rc = ifHCInUcastPkts_get(rowreq_ctx, (U64 *) var->val.string);
        break;

        /*
         * ifHCInMulticastPkts(8)/COUNTER64/ASN_COUNTER64/U64(U64)//l/A/w/e/r/d/h 
         */
    case COLUMN_IFHCINMULTICASTPKTS:
        var->val_len = sizeof(U64);
        var->type = ASN_COUNTER64;
        rc = ifHCInMulticastPkts_get(rowreq_ctx, (U64 *) var->val.string);
        break;

        /*
         * ifHCInBroadcastPkts(9)/COUNTER64/ASN_COUNTER64/U64(U64)//l/A/w/e/r/d/h 
         */
    case COLUMN_IFHCINBROADCASTPKTS:
        var->val_len = sizeof(U64);
        var->type = ASN_COUNTER64;
        rc = ifHCInBroadcastPkts_get(rowreq_ctx, (U64 *) var->val.string);
        break;

        /*
         * ifHCOutOctets(10)/COUNTER64/ASN_COUNTER64/U64(U64)//l/A/w/e/r/d/h 
         */
    case COLUMN_IFHCOUTOCTETS:
        var->val_len = sizeof(U64);
        var->type = ASN_COUNTER64;
        rc = ifHCOutOctets_get(rowreq_ctx, (U64 *) var->val.string);
        break;

        /*
         * ifHCOutUcastPkts(11)/COUNTER64/ASN_COUNTER64/U64(U64)//l/A/w/e/r/d/h 
         */
    case COLUMN_IFHCOUTUCASTPKTS:
        var->val_len = sizeof(U64);
        var->type = ASN_COUNTER64;
        rc = ifHCOutUcastPkts_get(rowreq_ctx, (U64 *) var->val.string);
        break;

        /*
         * ifHCOutMulticastPkts(12)/COUNTER64/ASN_COUNTER64/U64(U64)//l/A/w/e/r/d/h 
         */
    case COLUMN_IFHCOUTMULTICASTPKTS:
        var->val_len = sizeof(U64);
        var->type = ASN_COUNTER64;
        rc = ifHCOutMulticastPkts_get(rowreq_ctx, (U64 *) var->val.string);
        break;

        /*
         * ifHCOutBroadcastPkts(13)/COUNTER64/ASN_COUNTER64/U64(U64)//l/A/w/e/r/d/h 
         */
    case COLUMN_IFHCOUTBROADCASTPKTS:
        var->val_len = sizeof(U64);
        var->type = ASN_COUNTER64;
        rc = ifHCOutBroadcastPkts_get(rowreq_ctx, (U64 *) var->val.string);
        break;

        /*
         * ifLinkUpDownTrapEnable(14)/INTEGER/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h 
         */
    case COLUMN_IFLINKUPDOWNTRAPENABLE:
        var->val_len = sizeof(u_long);
        var->type = ASN_INTEGER;
        rc = ifLinkUpDownTrapEnable_get(rowreq_ctx,
                                        (u_long *) var->val.string);
        break;

        /*
         * ifHighSpeed(15)/GAUGE/ASN_GAUGE/u_long(u_long)//l/A/w/e/r/d/h 
         */
    case COLUMN_IFHIGHSPEED:
        var->val_len = sizeof(u_long);
        var->type = ASN_GAUGE;
        rc = ifHighSpeed_get(rowreq_ctx, (u_long *) var->val.string);
        break;

        /*
         * ifPromiscuousMode(16)/TruthValue/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h 
         */
    case COLUMN_IFPROMISCUOUSMODE:
        var->val_len = sizeof(u_long);
        var->type = ASN_INTEGER;
        rc = ifPromiscuousMode_get(rowreq_ctx, (u_long *) var->val.string);
        break;

        /*
         * ifConnectorPresent(17)/TruthValue/ASN_INTEGER/long(u_long)//l/A/w/E/r/d/h 
         */
    case COLUMN_IFCONNECTORPRESENT:
        var->val_len = sizeof(u_long);
        var->type = ASN_INTEGER;
        rc = ifConnectorPresent_get(rowreq_ctx,
                                    (u_long *) var->val.string);
        break;

        /*
         * ifAlias(18)/DisplayString/ASN_OCTET_STR/char(char)//L/A/W/e/R/d/H 
         */
    case COLUMN_IFALIAS:
        var->type = ASN_OCTET_STR;
        rc = ifAlias_get(rowreq_ctx, (char **) &var->val.string,
                         &var->val_len);
        break;

        /*
         * ifCounterDiscontinuityTime(19)/TimeStamp/ASN_TIMETICKS/u_long(u_long)//l/A/w/e/r/d/h 
         */
    case COLUMN_IFCOUNTERDISCONTINUITYTIME:
        var->val_len = sizeof(u_long);
        var->type = ASN_TIMETICKS;
        rc = ifCounterDiscontinuityTime_get(rowreq_ctx,
                                            (u_long *) var->val.string);
        break;

    }

    return rc;
}

int
_mfd_ifXTable_get_values(netsnmp_mib_handler *handler,
                         netsnmp_handler_registration *reginfo,
                         netsnmp_agent_request_info *agtreq_info,
                         netsnmp_request_info *requests)
{
    ifXTable_rowreq_ctx *rowreq_ctx =
        netsnmp_container_table_extract_context(requests);
    netsnmp_table_request_info *tri;
    u_char         *old_string;
    void            (*dataFreeHook) (void *);
    int             rc;

    DEBUGTRACE;

    netsnmp_assert(NULL != rowreq_ctx);

    for (; requests; requests = requests->next) {
        /*
         * save old pointer, so we can free it if replaced
         */
        old_string = requests->requestvb->val.string;
        dataFreeHook = requests->requestvb->dataFreeHook;
        if (NULL == requests->requestvb->val.string) {
            requests->requestvb->val.string = requests->requestvb->buf;
            requests->requestvb->val_len =
                sizeof(requests->requestvb->buf);
        } else if (requests->requestvb->buf ==
                   requests->requestvb->val.string) {
            if (requests->requestvb->val_len !=
                sizeof(requests->requestvb->buf))
                requests->requestvb->val_len =
                    sizeof(requests->requestvb->buf);
        }

        /*
         * get column data
         */
        tri = netsnmp_extract_table_info(requests);
        if (NULL == tri)
            continue;

        rc = _ifXTable_get_column(rowreq_ctx, requests->requestvb,
                                  tri->colnum);
        if (rc) {
            if (MFD_SKIP == rc) {
                requests->requestvb->type = ASN_PRIV_RETRY;
                rc = SNMP_ERR_NOERROR;
            }
        } else if (NULL == requests->requestvb->val.string) {
            snmp_log(LOG_ERR, "NULL varbind data pointer!\n");
            rc = SNMP_ERR_GENERR;
        }
        if (rc)
            netsnmp_request_set_error(requests, rc);

        /*
         * if the buffer wasn't used previously for the old data (i.e. it
         * was allcoated memory)  and the get routine replaced the pointer,
         * we need to free the previous pointer.
         */
        if (old_string && (old_string != requests->requestvb->buf) &&
            (requests->requestvb->val.string != old_string)) {
            if (dataFreeHook)
                (*dataFreeHook) (old_string);
            else
                free(old_string);
        }
    }                           /* for results */

    return SNMP_ERR_NOERROR;
}

/***********************************************************************
 *
 * SET processing
 *
 ***********************************************************************/

/*----------------------------------------------------------------------
 *
 * SET: Syntax checks
 *
 *---------------------------------------------------------------------*/
/*
 * @internal
 * Check the syntax for a particular column
 */
NETSNMP_STATIC_INLINE int
_ifXTable_check_column(ifXTable_rowreq_ctx * rowreq_ctx,
                       netsnmp_variable_list * var, int column)
{
    int             rc = SNMPERR_SUCCESS;

    DEBUGTRACE;

    netsnmp_assert(NULL != rowreq_ctx);

    switch (column) {

        /*
         * ifLinkUpDownTrapEnable(14)/INTEGER/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h 
         */
    case COLUMN_IFLINKUPDOWNTRAPENABLE:
        /** no size to check ($m2c_nvv_sizeof not set) */
        rc = netsnmp_check_vb_type(var, ASN_INTEGER);
        if (SNMPERR_SUCCESS == rc) {

            /*
             * check that the value is one of defined enums 
             */
            if (1 && (*var->val.integer != IFLINKUPDOWNTRAPENABLE_ENABLED)
                && (*var->val.integer != IFLINKUPDOWNTRAPENABLE_DISABLED)
                ) {
                rc = SNMP_ERR_BADVALUE;
            }
        }
        if (SNMPERR_SUCCESS == rc) {
            rc = ifLinkUpDownTrapEnable_check_value(rowreq_ctx,
                                                    *((u_long *) var->val.
                                                      string));
            if (rc)
                rc = SNMP_ERR_BADVALUE;
        }
        break;

        /*
         * ifPromiscuousMode(16)/TruthValue/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h 
         */
    case COLUMN_IFPROMISCUOUSMODE:
        /** no size to check ($m2c_nvv_sizeof not set) */
        rc = netsnmp_check_vb_type(var, ASN_INTEGER);
        if (SNMPERR_SUCCESS == rc) {

            /*
             * check that the value is one of defined enums 
             */
            if (1 && (*var->val.integer != TRUTHVALUE_TRUE)
                && (*var->val.integer != TRUTHVALUE_FALSE)
                ) {
                rc = SNMP_ERR_BADVALUE;
            }
        }
        if (SNMPERR_SUCCESS == rc) {
            rc = ifPromiscuousMode_check_value(rowreq_ctx,
                                               *((u_long *) var->val.
                                                 string));
            if (rc)
                rc = SNMP_ERR_BADVALUE;
        }
        break;

        /*
         * ifAlias(18)/DisplayString/ASN_OCTET_STR/char(char)//L/A/W/e/R/d/H 
         */
    case COLUMN_IFALIAS:
        /** no size to check ($m2c_nvv_sizeof not set) */
        rc = netsnmp_check_vb_type(var, ASN_OCTET_STR);
        if (SNMPERR_SUCCESS == rc) {
            /*
             * check that the value is in the defined range(s); inefficent
             * * but keeps rc value knowledge in libarary where it belongs. 
             */
            if (1
                && ((rc = netsnmp_check_vb_size_range(var, 0, 64)) !=
                    SNMP_ERR_NOERROR)
                ) {
                ;               /* rc set in condition */
            }
        }
        if (SNMPERR_SUCCESS == rc) {
            rc = ifAlias_check_value(rowreq_ctx, (char *) var->val.string,
                                     var->val_len);
            if (rc)
                rc = SNMP_ERR_BADVALUE;
        }
        break;

    default:    /** We shouldn't get here */
        rc = SNMP_ERR_GENERR;
        snmp_log(LOG_ERR, "unknown column in _ifXTable_check_column\n");
    }

    return rc;
}

int
_mfd_ifXTable_check_objects(netsnmp_mib_handler *handler,
                            netsnmp_handler_registration *reginfo,
                            netsnmp_agent_request_info *agtreq_info,
                            netsnmp_request_info *requests)
{
    ifXTable_rowreq_ctx *rowreq_ctx =
        netsnmp_container_table_extract_context(requests);
    netsnmp_table_request_info *tri;
    int             rc;

    DEBUGTRACE;

    netsnmp_assert(NULL != rowreq_ctx);

    for (; requests; requests = requests->next) {

        /*
         * get column number from table request info, and check that column
         */
        tri = netsnmp_extract_table_info(requests);
        if (NULL == tri)
            continue;

        rc = _ifXTable_check_column(rowreq_ctx, requests->requestvb,
                                    tri->colnum);
        if (rc) {
            netsnmp_request_set_error(requests, rc);
            break;
        }

    }                           /* for results */

    return rc;
}



/*----------------------------------------------------------------------
 *
 * SET: check dependencies
 *
 *---------------------------------------------------------------------*/
/*
 * @internal
 * Check dependencies wrapper
 */
static int
_mfd_ifXTable_check_dependencies(netsnmp_mib_handler *handler,
                                 netsnmp_handler_registration *reginfo,
                                 netsnmp_agent_request_info *agtreq_info,
                                 netsnmp_request_info *requests)
{
    ifXTable_rowreq_ctx *rowreq_ctx =
        netsnmp_container_table_extract_context(requests);
    DEBUGTRACE;

    netsnmp_assert(NULL != rowreq_ctx);

    return ifXTable_check_dependencies(rowreq_ctx);
}

/*----------------------------------------------------------------------
 *
 * SET: Undo setup
 *
 *---------------------------------------------------------------------*/
/**
 * @internal
 * undo setup
 */
int
_mfd_ifXTable_undo_setup(netsnmp_mib_handler *handler,
                         netsnmp_handler_registration *reginfo,
                         netsnmp_agent_request_info *agtreq_info,
                         netsnmp_request_info *requests)
{
    ifXTable_rowreq_ctx *rowreq_ctx =
        netsnmp_container_table_extract_context(requests);
    netsnmp_assert(NULL != rowreq_ctx);
    return ifXTable_undo_setup(rowreq_ctx);
}

/*----------------------------------------------------------------------
 *
 * SET: Set values
 *
 *---------------------------------------------------------------------*/
/*
 * @internal
 * Set the value for a particular column
 */
NETSNMP_STATIC_INLINE int
_ifXTable_set_column(ifXTable_rowreq_ctx * rowreq_ctx,
                     netsnmp_variable_list * var, int column)
{
    int             rc = SNMPERR_SUCCESS;

    DEBUGTRACE;

    netsnmp_assert(NULL != rowreq_ctx);

    switch (column) {

        /*
         * ifLinkUpDownTrapEnable(14)/INTEGER/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h 
         */
    case COLUMN_IFLINKUPDOWNTRAPENABLE:
        if (var->val_len != sizeof(u_long)) {
            rc = MFD_ERROR;
            snmp_log(LOG_ERR,
                     "varbind size of %d does not match expected size %d\n",
                     var->val_len, sizeof(u_long));
            break;
        }
        rc = ifLinkUpDownTrapEnable_set(rowreq_ctx,
                                        *((u_long *) var->val.string));
        break;

        /*
         * ifPromiscuousMode(16)/TruthValue/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h 
         */
    case COLUMN_IFPROMISCUOUSMODE:
        if (var->val_len != sizeof(u_long)) {
            rc = MFD_ERROR;
            snmp_log(LOG_ERR,
                     "varbind size of %d does not match expected size %d\n",
                     var->val_len, sizeof(u_long));
            break;
        }
        rc = ifPromiscuousMode_set(rowreq_ctx,
                                   *((u_long *) var->val.string));
        break;

        /*
         * ifAlias(18)/DisplayString/ASN_OCTET_STR/char(char)//L/A/W/e/R/d/H 
         */
    case COLUMN_IFALIAS:
        rc = ifAlias_set(rowreq_ctx, (char *) var->val.string,
                         var->val_len);
        break;

    }

    return rc;
}

int
_mfd_ifXTable_set_values(netsnmp_mib_handler *handler,
                         netsnmp_handler_registration *reginfo,
                         netsnmp_agent_request_info *agtreq_info,
                         netsnmp_request_info *requests)
{
    ifXTable_rowreq_ctx *rowreq_ctx =
        netsnmp_container_table_extract_context(requests);
    netsnmp_table_request_info *tri;
    int             rc = SNMP_ERR_NOERROR;

    DEBUGTRACE;

    netsnmp_assert(NULL != rowreq_ctx);

    for (; requests; requests = requests->next) {
        /*
         * set column data
         */
        tri = netsnmp_extract_table_info(requests);
        if (NULL == tri)
            continue;

        _ifXTable_set_column(rowreq_ctx, requests->requestvb, tri->colnum);

    }                           /* for results */

    return rc;
}

/*----------------------------------------------------------------------
 *
 * SET: commit
 *
 *---------------------------------------------------------------------*/
/**
 * @internal
 * commit the values
 */
int
_mfd_ifXTable_commit(netsnmp_mib_handler *handler,
                     netsnmp_handler_registration *reginfo,
                     netsnmp_agent_request_info *agtreq_info,
                     netsnmp_request_info *requests)
{
    ifXTable_rowreq_ctx *rowreq_ctx =
        netsnmp_container_table_extract_context(requests);
    netsnmp_assert(NULL != rowreq_ctx);
    return ifXTable_commit(rowreq_ctx);
}

/*----------------------------------------------------------------------
 *
 * SET: Undo
 *
 *---------------------------------------------------------------------*/
/**
 * @internal
 * undo the value for a particular column
 */
NETSNMP_STATIC_INLINE int
_ifXTable_undo_column(ifXTable_rowreq_ctx * rowreq_ctx,
                      netsnmp_variable_list * var, int column)
{
    int             rc = SNMPERR_SUCCESS;

    DEBUGTRACE;

    netsnmp_assert(NULL != rowreq_ctx);

    switch (column) {

        /*
         * ifLinkUpDownTrapEnable(14)/INTEGER/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h 
         */
    case COLUMN_IFLINKUPDOWNTRAPENABLE:
        rc = ifLinkUpDownTrapEnable_undo(rowreq_ctx);
        break;

        /*
         * ifPromiscuousMode(16)/TruthValue/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h 
         */
    case COLUMN_IFPROMISCUOUSMODE:
        rc = ifPromiscuousMode_undo(rowreq_ctx);
        break;

        /*
         * ifAlias(18)/DisplayString/ASN_OCTET_STR/char(char)//L/A/W/e/R/d/H 
         */
    case COLUMN_IFALIAS:
        rc = ifAlias_undo(rowreq_ctx);
        break;

    }

    return rc;
}

int
_mfd_ifXTable_undo_values(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info *agtreq_info,
                          netsnmp_request_info *requests)
{
    ifXTable_rowreq_ctx *rowreq_ctx =
        netsnmp_container_table_extract_context(requests);
    netsnmp_table_request_info *tri;
    int             rc = SNMP_ERR_NOERROR;

    DEBUGTRACE;

    netsnmp_assert(NULL != rowreq_ctx);

    for (; requests; requests = requests->next) {
        /*
         * set column data
         */
        tri = netsnmp_extract_table_info(requests);
        if (NULL == tri)
            continue;

        _ifXTable_undo_column(rowreq_ctx, requests->requestvb,
                              tri->colnum);

    }                           /* for results */

    return rc;
}

/***********************************************************************
 *
 * DATA ACCESS
 *
 ***********************************************************************/
/**
 * @internal
 */
static int
_cache_load(netsnmp_cache * cache, void *vmagic)
{
    DEBUGTRACE;

    if ((NULL == cache) || (NULL == cache->magic)) {
        snmp_log(LOG_ERR, "invalid cache for ifXTable_cache_load\n");
        return -1;
    }
    DEBUGMSGTL(("ifTable/cache", "ifTable_load %p/%p\n",
                cache, cache->magic));

    netsnmp_assert(0 == cache->valid);

    /*
     * call user code
     */
    return ifXTable_cache_load((netsnmp_container *) cache->magic);
}

/**
 * @internal
 */
static void
_cache_item_free(ifXTable_rowreq_ctx * rowreq_ctx, void *context)
{
    DEBUGTRACE;

    if (NULL == rowreq_ctx)
        return;

    ifXTable_release_rowreq_ctx(rowreq_ctx);
}

/**
 * @internal
 */
static void
_cache_free(netsnmp_cache * cache, void *magic)
{
    netsnmp_container *container;

    DEBUGTRACE;

    if ((NULL == cache) || (NULL == cache->magic)) {
        snmp_log(LOG_ERR, "invalid cache in ifXTable_cache_free\n");
        return;
    }

    container = (netsnmp_container *) cache->magic;

    /*
     * call user code
     */
    ifXTable_cache_free(container);

    /*
     * free all items. inefficient, but easy.
     */
    CONTAINER_CLEAR(container,
                    (netsnmp_container_obj_func *) _cache_item_free, NULL);
}

/**
 * @internal
 * initialize the iterator container with functions or wrappers
 */
void
_ifXTable_container_init(ifXTable_interface_ctx * if_ctx)
{
    extern oid             ifTable_oid;
    extern int             ifTable_oid_size;

    DEBUGTRACE;

    /*
     * set up the cache
     * special case: sharing a cache.
     */
    if_ctx->cache = netsnmp_cache_find_by_oid(&ifTable_oid, ifTable_oid_size);
    if (NULL != if_ctx->cache) {
        if_ctx->container = (netsnmp_container*)if_ctx->cache->magic;
        return;
    }

    snmp_log(LOG_ERR, "error finding cache for ifTable, using new one\n");
    if_ctx->cache = netsnmp_cache_create(30,    /* timeout in seconds */
                                         _cache_load, _cache_free,
                                         ifXTable_oid, ifXTable_oid_size);
    if (NULL == if_ctx->cache) {
        snmp_log(LOG_ERR, "error creating cache for ifXTable\n");
        return;
    }

    ifXTable_container_init(&if_ctx->container, if_ctx->cache);
    if (NULL == if_ctx->container)
        if_ctx->container =
            netsnmp_container_find("ifXTable:table_container");
    if (NULL == if_ctx->container) {
        snmp_log(LOG_ERR, "error creating container in "
                 "ifXTable_container_init\n");
        return;
    }
    if_ctx->cache->magic = (void *) if_ctx->container;
}
