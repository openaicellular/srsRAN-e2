/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2SM-gNB-NRT-IEs"
 * 	found in "/local/mnt/openairinterface5g/openair2/RIC_AGENT/MESSAGES/ASN1/R01/e2sm-gNB-NRT-v401.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-PER -no-gen-OER -no-gen-example -fno-include-deps -fincludes-quoted -D /local/mnt/openairinterface5g/cmake_targets/ran_build/build/CMakeFiles/E2SM-GNB-NRT/`
 */

#ifndef	_E2SM_GNB_NRT_NRT_ControlCommand_H_
#define	_E2SM_GNB_NRT_NRT_ControlCommand_H_


#include "asn_application.h"

/* Including external dependencies */
#include "NativeEnumerated.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum E2SM_GNB_NRT_NRT_ControlCommand {
	E2SM_GNB_NRT_NRT_ControlCommand_addition	= 0,
	E2SM_GNB_NRT_NRT_ControlCommand_deletion	= 1
	/*
	 * Enumeration is extensible
	 */
} e_E2SM_GNB_NRT_NRT_ControlCommand;

/* E2SM_GNB_NRT_NRT-ControlCommand */
typedef long	 E2SM_GNB_NRT_NRT_ControlCommand_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_E2SM_GNB_NRT_NRT_ControlCommand_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_E2SM_GNB_NRT_NRT_ControlCommand;
extern const asn_INTEGER_specifics_t asn_SPC_E2SM_GNB_NRT_NRT_ControlCommand_specs_1;
asn_struct_free_f E2SM_GNB_NRT_NRT_ControlCommand_free;
asn_struct_print_f E2SM_GNB_NRT_NRT_ControlCommand_print;
asn_constr_check_f E2SM_GNB_NRT_NRT_ControlCommand_constraint;
ber_type_decoder_f E2SM_GNB_NRT_NRT_ControlCommand_decode_ber;
der_type_encoder_f E2SM_GNB_NRT_NRT_ControlCommand_encode_der;
xer_type_decoder_f E2SM_GNB_NRT_NRT_ControlCommand_decode_xer;
xer_type_encoder_f E2SM_GNB_NRT_NRT_ControlCommand_encode_xer;
per_type_decoder_f E2SM_GNB_NRT_NRT_ControlCommand_decode_uper;
per_type_encoder_f E2SM_GNB_NRT_NRT_ControlCommand_encode_uper;
per_type_decoder_f E2SM_GNB_NRT_NRT_ControlCommand_decode_aper;
per_type_encoder_f E2SM_GNB_NRT_NRT_ControlCommand_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _E2SM_GNB_NRT_NRT_ControlCommand_H_ */
#include "asn_internal.h"
