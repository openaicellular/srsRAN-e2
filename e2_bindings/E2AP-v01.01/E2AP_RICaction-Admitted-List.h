/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2AP-PDU-Contents"
 * 	found in "./e2ap-v01.01.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-PER -no-gen-OER -no-gen-example -fno-include-deps -fincludes-quoted -D E2AP-v01.01/`
 */

#ifndef	_E2AP_RICaction_Admitted_List_H_
#define	_E2AP_RICaction_Admitted_List_H_


#include "asn_application.h"

/* Including external dependencies */
#include "asn_SEQUENCE_OF.h"
#include "constr_SEQUENCE_OF.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct E2AP_ProtocolIE_SingleContainer;

/* E2AP_RICaction-Admitted-List */
typedef struct E2AP_RICaction_Admitted_List {
	A_SEQUENCE_OF(struct E2AP_ProtocolIE_SingleContainer) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} E2AP_RICaction_Admitted_List_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_E2AP_RICaction_Admitted_List;
extern asn_SET_OF_specifics_t asn_SPC_E2AP_RICaction_Admitted_List_specs_1;
extern asn_TYPE_member_t asn_MBR_E2AP_RICaction_Admitted_List_1[1];
extern asn_per_constraints_t asn_PER_type_E2AP_RICaction_Admitted_List_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _E2AP_RICaction_Admitted_List_H_ */
#include "asn_internal.h"
