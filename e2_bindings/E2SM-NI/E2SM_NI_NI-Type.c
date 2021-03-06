/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E2SM-NI-IEs"
 * 	found in "/local/mnt/openairinterface5g/openair2/RIC_AGENT/MESSAGES/ASN1/R01/e2sm-ni-v01.00.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-PER -no-gen-OER -no-gen-example -fno-include-deps -fincludes-quoted -D /local/mnt/openairinterface5g/cmake_targets/ran_build/build/CMakeFiles/E2SM-NI/`
 */

#include "E2SM_NI_NI-Type.h"

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
asn_per_constraints_t asn_PER_type_E2SM_NI_NI_Type_constr_1 CC_NOTUSED = {
	{ APC_CONSTRAINED | APC_EXTENSIBLE,  3,  3,  0,  5 }	/* (0..5,...) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_E2SM_NI_NI_Type_value2enum_1[] = {
	{ 0,	2,	"s1" },
	{ 1,	2,	"x2" },
	{ 2,	2,	"ng" },
	{ 3,	2,	"xn" },
	{ 4,	2,	"f1" },
	{ 5,	2,	"e1" }
	/* This list is extensible */
};
static const unsigned int asn_MAP_E2SM_NI_NI_Type_enum2value_1[] = {
	5,	/* e1(5) */
	4,	/* f1(4) */
	2,	/* ng(2) */
	0,	/* s1(0) */
	1,	/* x2(1) */
	3	/* xn(3) */
	/* This list is extensible */
};
const asn_INTEGER_specifics_t asn_SPC_E2SM_NI_NI_Type_specs_1 = {
	asn_MAP_E2SM_NI_NI_Type_value2enum_1,	/* "tag" => N; sorted by tag */
	asn_MAP_E2SM_NI_NI_Type_enum2value_1,	/* N => "tag"; sorted by N */
	6,	/* Number of elements in the maps */
	7,	/* Extensions before this member */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_E2SM_NI_NI_Type_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
asn_TYPE_descriptor_t asn_DEF_E2SM_NI_NI_Type = {
	"NI-Type",
	"NI-Type",
	&asn_OP_NativeEnumerated,
	asn_DEF_E2SM_NI_NI_Type_tags_1,
	sizeof(asn_DEF_E2SM_NI_NI_Type_tags_1)
		/sizeof(asn_DEF_E2SM_NI_NI_Type_tags_1[0]), /* 1 */
	asn_DEF_E2SM_NI_NI_Type_tags_1,	/* Same as above */
	sizeof(asn_DEF_E2SM_NI_NI_Type_tags_1)
		/sizeof(asn_DEF_E2SM_NI_NI_Type_tags_1[0]), /* 1 */
	{ 0, &asn_PER_type_E2SM_NI_NI_Type_constr_1, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_E2SM_NI_NI_Type_specs_1	/* Additional specs */
};

