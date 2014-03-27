/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford
* Junior University
* Copyright (c) 2011, 2012 Open Networking Foundation
*
* We are making the OpenFlow specification and associated documentation
* (Software) available for public use and benefit with the expectation
* that others will use, modify and enhance the Software and contribute
* those enhancements back to the community. However, since we would
* like to make the Software available for broadest use, with as few
* restrictions as possible permission is hereby granted, free of
* charge, to any person obtaining a copy of this Software to deal in
* the Software under the copyrights without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
* BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
* ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* The name and trademarks of copyright holder(s) may NOT be used in
* advertising or publicity pertaining to the Software or any
* derivatives without specific, written prior permission.
*/

/* OpenFlow: protocol between controller and datapath. */

#ifndef OPENFLOW_14_H
#define OPENFLOW_14_H 1

#include "openflow/openflow-1.3.h"
#include "openvswitch/types.h"
/*
 * OpenFlow 1.4 is more extensible by using TLV structures
 */

/* Common header for all async config Properties */
struct ofp14_async_config_prop_header {
    ovs_be16    type;       /* One of OFPACPT_*. */
    ovs_be16    length;     /* Length in bytes of this property. */
};
OFP_ASSERT(sizeof(struct ofp14_async_config_prop_header) == 4);

/* Asynchronous message configuration.
 * OFPT_GET_ASYNC_REPLY or OFPT_SET_ASYNC.
 */
struct ofp14_async_config {
    struct ofp_header header;
    /* Async config Property list - 0 or more */
    struct ofp14_async_config_prop_header properties[0];
};
OFP_ASSERT(sizeof(struct ofp14_async_config) == 8);

/* Async Config property types.
* Low order bit cleared indicates a property for the slave role.
* Low order bit set indicates a property for the master/equal role.
*/
enum ofp14_async_config_prop_type {
    OFPACPT_PACKET_IN_SLAVE       = 0, /* Packet-in mask for slave. */
    OFPACPT_PACKET_IN_MASTER      = 1, /* Packet-in mask for master. */
    OFPACPT_PORT_STATUS_SLAVE     = 2, /* Port-status mask for slave. */
    OFPACPT_PORT_STATUS_MASTER    = 3, /* Port-status mask for master. */
    OFPACPT_FLOW_REMOVED_SLAVE    = 4, /* Flow removed mask for slave. */
    OFPACPT_FLOW_REMOVED_MASTER   = 5, /* Flow removed mask for master. */
    OFPACPT_ROLE_STATUS_SLAVE     = 6, /* Role status mask for slave. */
    OFPACPT_ROLE_STATUS_MASTER    = 7, /* Role status mask for master. */
    OFPACPT_TABLE_STATUS_SLAVE    = 8, /* Table status mask for slave. */
    OFPACPT_TABLE_STATUS_MASTER   = 9, /* Table status mask for master. */
    OFPACPT_REQUESTFORWARD_SLAVE  = 10, /* RequestForward mask for slave. */
    OFPACPT_REQUESTFORWARD_MASTER = 11, /* RequestForward mask for master. */
    OFPTFPT_EXPERIMENTER_SLAVE    = 0xFFFE, /* Experimenter for slave. */
    OFPTFPT_EXPERIMENTER_MASTER   = 0xFFFF, /* Experimenter for master. */
};

/* Various reason based properties */
struct ofp14_async_config_prop_reasons {
    /* 'type' is one of OFPACPT_PACKET_IN_*, OFPACPT_PORT_STATUS_*,
     * OFPACPT_FLOW_REMOVED_*, OFPACPT_ROLE_STATUS_*,
     * OFPACPT_TABLE_STATUS_*, OFPACPT_REQUESTFORWARD_*. */
    ovs_be16    type;
    ovs_be16    length; /* Length in bytes of this property. */
    ovs_be32    mask;   /* Bitmasks of reason values. */
};
OFP_ASSERT(sizeof(struct ofp14_async_config_prop_reasons) == 8);

/* Experimenter async config property */
struct ofp14_async_config_prop_experimenter {
    ovs_be16        type;       /* One of OFPTFPT_EXPERIMENTER_SLAVE,
                                   OFPTFPT_EXPERIMENTER_MASTER. */
    ovs_be16        length;     /* Length in bytes of this property. */
    ovs_be32        experimenter;  /* Experimenter ID which takes the same
                                      form as in struct
                                      ofp_experimenter_header. */
    ovs_be32        exp_type;      /* Experimenter defined. */
    /* Followed by:
     *   - Exactly (length - 12) bytes containing the experimenter data, then
     *   - Exactly (length + 7)/8*8 - (length) (between 0 and 7)
     *     bytes of all-zero bytes */
};
OFP_ASSERT(sizeof(struct ofp14_async_config_prop_experimenter) == 12);

/* Common header for all Role Properties */
struct ofp14_role_prop_header {
    ovs_be16 type;   /* One of OFPRPT_*. */
    ovs_be16 length; /* Length in bytes of this property. */
};
OFP_ASSERT(sizeof(struct ofp14_role_prop_header) == 4);

/* Role status event message. */
struct ofp14_role_status {
    ovs_be32 role;              /* One of OFPCR_ROLE_*. */
    uint8_t  reason;            /* One of OFPCRR_*. */
    uint8_t  pad[3];            /* Align to 64 bits. */
    ovs_be64 generation_id;     /* Master Election Generation Id */

    /* Followed by a list of struct ofp14_role_prop_header */
};
OFP_ASSERT(sizeof(struct ofp14_role_status) == 16);

/* What changed about the controller role */
enum ofp14_controller_role_reason {
    OFPCRR_MASTER_REQUEST = 0,  /* Another controller asked to be master. */
    OFPCRR_CONFIG         = 1,  /* Configuration changed on the switch. */
    OFPCRR_EXPERIMENTER   = 2,  /* Experimenter data changed. */
};

/* Role property types.
*/
enum ofp14_role_prop_type {
    OFPRPT_EXPERIMENTER         = 0xFFFF, /* Experimenter property. */
};

/* Experimenter role property */
struct ofp14_role_prop_experimenter {
    ovs_be16        type;       /* One of OFPRPT_EXPERIMENTER. */
    ovs_be16        length;     /* Length in bytes of this property. */
    ovs_be32        experimenter; /* Experimenter ID which takes the same
                                     form as in struct
                                     ofp_experimenter_header. */
    ovs_be32        exp_type;     /* Experimenter defined. */
    /* Followed by:
     *   - Exactly (length - 12) bytes containing the experimenter data, then
     *   - Exactly (length + 7)/8*8 - (length) (between 0 and 7)
     *     bytes of all-zero bytes */
};
OFP_ASSERT(sizeof(struct ofp14_role_prop_experimenter) == 12);

/* Bundle control message types */
enum ofp14_bundle_ctrl_type {
    OFPBCT_OPEN_REQUEST    = 0,
    OFPBCT_OPEN_REPLY      = 1,
    OFPBCT_CLOSE_REQUEST   = 2,
    OFPBCT_CLOSE_REPLY     = 3,
    OFPBCT_COMMIT_REQUEST  = 4,
    OFPBCT_COMMIT_REPLY    = 5,
    OFPBCT_DISCARD_REQUEST = 6,
    OFPBCT_DISCARD_REPLY   = 7,
};

/* Bundle configuration flags. */
enum ofp14_bundle_flags {
    OFPBF_ATOMIC  = 1 << 0,  /* Execute atomically. */
    OFPBF_ORDERED = 1 << 1,  /* Execute in specified order. */
};

/* Message structure for ONF_ET_BUNDLE_CONTROL. */
struct ofp14_bundle_ctrl_msg {
    ovs_be32 bundle_id;     /* Identify the bundle. */
    ovs_be16 type;          /* OFPBCT_*. */
    ovs_be16 flags;         /* Bitmap of OFPBF_* flags. */
    /* Bundle Property list. */
    /* struct ofp14_bundle_prop_header properties[0]; */
};
OFP_ASSERT(sizeof(struct ofp14_bundle_ctrl_msg) == 8);

/* Message structure for OFP_BUNDLE_ADD_MESSAGE.
* Adding a message in a bundle is done with. */
struct ofp14_bundle_add_msg {
    ovs_be32            bundle_id;  /* Identify the bundle. */
    uint8_t             pad[2];     /* Align to 64 bits. */
    ovs_be16            flags;      /* Bitmap of ONF_BF_* flags. */

    struct ofp_header   message;    /* Message added to the bundle. */

    /* If there is one property or more, 'message' is followed by:
     * - Exactly (message.length + 7)/8*8 - (message.length) (between 0 and 7)
     *   bytes of all-zero bytes */

    /* Bundle Property list. */
    /* struct ofp14_bundle_prop_header properties[0]; */
};
OFP_ASSERT(sizeof(struct ofp14_bundle_add_msg) == 16);

struct ofp14_match {
	uint16_t type; /* One of OFPMT_* */
	uint16_t length; /* Length of ofp_match (excluding padding) */
	/* Followed by:
	* - Exactly (length - 4) (possibly 0) bytes containing OXM TLVs, then
	* - Exactly ((length + 7)/8*8 - length) (between 0 and 7) bytes of
	* all-zero bytes
	* In summary, ofp_match is padded as needed, to make its overall size
	* a multiple of 8, to preserve alignement in structures using it.
	*/
	uint8_t oxm_fields[0]; /* 0 or more OXM match fields */
	uint8_t pad[4]; /* Zero bytes - see above for sizing */
};
OFP_ASSERT(sizeof(struct ofp14_match) == 8);

struct ofp14_flow_monitor_request {
	uint32_t monitor_id; /* Controller-assigned ID for this monitor. */
	uint32_t out_port; /* Required output port, if not OFPP_ANY. */
	uint32_t out_group; /* Required output port, if not OFPG_ANY. */
	uint16_t flags; /* OFFMF_*. */
	uint8_t table_id; /* One table's ID or OFPTT_ALL (all tables). */
	uint8_t command; /* One of OFPFMC_*. */
	struct ofp14_match match; /* Fields to match. Variable size. */
};
OFP_ASSERT(sizeof(struct ofp14_flow_monitor_request) == 24);


/* Flow monitor commands */
enum ofp_flow_monitor_command {
OFPFMC_ADD = 0, /* New flow monitor. */
OFPFMC_MODIFY = 1, /* Modify existing flow monitor. */
OFPFMC_DELETE = 2, /* Delete/cancel existing flow monitor. */
};

/* 'flags' bits in struct of_flow_monitor_request. */
enum ofp14_flow_monitor_flags {
/* When to send updates. */
OFPFMF_INITIAL = 1 << 0, /* Initially matching flows. */
OFPFMF_ADD = 1 << 1, /* New matching flows as they are added. */
OFPFMF_REMOVED = 1 << 2, /* Old matching flows as they are removed. */
OFPFMF_MODIFY = 1 << 3, /* Matching flows as they are changed. */
/* What to include in updates. */
OFPFMF_INSTRUCTIONS = 1 << 4,/* If set, instructions are included. */
OFPFMF_NO_ABBREV = 1 << 5, /* If set, include own changes in full. */
OFPFMF_ONLY_OWN = 1 << 6, /* If set, don't include other controllers. */
};


/* OFPMP_FLOW_MONITOR reply header.
*
* The body of an OFPMP_FLOW_MONITOR reply is an array of variable-length
* structures, each of which begins with this header. The 'length' member may
* be used to traverse the array, and the 'event' member may be used to
* determine the particular structure.
*
* Every instance is a multiple of 8 bytes long. */
struct ofp14_flow_update_header {
uint16_t length; /* Length of this entry. */
uint16_t event; /* One of OFPFME_*. */
/* ...other data depending on 'event'... */
};
OFP_ASSERT(sizeof(struct ofp14_flow_update_header) == 4);

/* 'event' values in struct ofp_flow_update_header. */
enum ofp14_flow_update_event {
/* struct ofp_flow_update_full. */
OFPFME_INITIAL = 0, /* Flow present when flow monitor created. */
OFPFME_ADDED = 1, /* Flow was added. */
OFPFME_REMOVED = 2, /* Flow was removed. */
OFPFME_MODIFIED = 3, /* Flow instructions were changed. */
/* struct ofp_flow_update_abbrev. */
OFPFME_ABBREV = 4, /* Abbreviated reply. */
/* struct ofp_flow_update_header. */
OFPFME_PAUSED = 5, /* Monitoring paused (out of buffer space). */
OFPFME_RESUMED = 6, /* Monitoring resumed. */
};

/* OFPMP_FLOW_MONITOR reply for OFPFME_INITIAL, OFPFME_ADDED, OFPFME_REMOVED,
* and OFPFME_MODIFIED. */
struct ofp14_flow_update_full {
uint16_t length; /* Length is 32 + match + instructions. */
uint16_t event; /* One of OFPFME_*. */
uint8_t table_id; /* ID of flow's table. */
uint8_t reason; /* OFPRR_* for OFPFME_REMOVED, else zero. */
uint16_t idle_timeout; /* Number of seconds idle before expiration. */
uint16_t hard_timeout; /* Number of seconds before expiration. */
uint16_t priority; /* Priority of the entry. */
uint8_t zeros[4]; /* Reserved, currently zeroed. */
uint64_t cookie; /* Opaque controller-issued identifier. */
struct ofp14_match match;
};
OFP_ASSERT(sizeof(struct ofp14_flow_update_full) == 32);


/* OFPMP_FLOW_MONITOR reply for OFPFME_ABBREV.
*
* When the controller does not specify OFPFMF_OWN in a monitor request, any
* flow tables changes due to the controller's own requests (on the same
* OpenFlow channel) will be abbreviated, when possible, to this form, which
* simply specifies the 'xid' of the OpenFlow request (e.g. an OFPT_FLOW_MOD)
* that caused the change.
* Some changes cannot be abbreviated and will be sent in full.
*/
struct ofp14_flow_update_abbrev {
	uint16_t length; /* Length is 8. */
	uint16_t event; /* OFPFME_ABBREV. */
	uint32_t xid; /* Controller-specified xid from flow_mod. */
	};
	OFP_ASSERT(sizeof(struct ofp14_flow_update_abbrev) == 8);


	/* OFPMP_FLOW_MONITOR reply for OFPFME_PAUSED and OFPFME_RESUMED.
	*/
	struct ofp14_flow_update_paused {
	uint16_t length; /* Length is 8. */
	uint16_t event; /* One of OFPFME_*. */
	uint8_t zeros[4]; /* Reserved, currently zeroed. */
	};
	OFP_ASSERT(sizeof(struct ofp14_flow_update_paused) == 8);


#endif /* openflow/openflow-1.4.h */
