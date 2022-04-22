#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __linux__
#define EXPORT
#else
#endif

#ifdef __cplusplus
extern "C" {
#endif

	#define REG_FIELD_NAME_LEN (72)

	typedef enum TYPE_E
	{
		REGISTER = 0,
		FIELD = 1,
	} TYPE;

	typedef struct
	{
		char name[REG_FIELD_NAME_LEN];
		uint32_t adress;
		uint32_t offset;
		uint32_t mask;
	} RegFieldInfo;

	typedef enum COLOR_E
	{
		BLACK = 0,
		RED = 1,
	} COLOR;

	typedef struct RBTreeNodeS
	{
		RegFieldInfo* data;

		COLOR color;
		struct RBTreeNodeS* left;
		struct RBTreeNodeS* right;
		struct RBTreeNodeS* parent;
	} RBTreeNode;

	EXPORT void va7044_A0_regs_init();
	EXPORT void va7044_A0_regs_destroy();
	EXPORT bool va7044_A0_regs_get(const char* pFieldName, RegFieldInfo* pItem);

	EXPORT void rb_tree_init(RBTreeNode** pRoot);
	EXPORT bool rb_tree_destroy(RBTreeNode** pRoot);
	EXPORT bool rb_tree_insert(RBTreeNode** pRoot, RegFieldInfo* pFiled);
	EXPORT bool rb_tree_get(RBTreeNode* pRoot, const char* name, RegFieldInfo* pField);
	EXPORT bool is_NIL_same(RBTreeNode* pNIL);

	// not debugged yet
	// EXPORT bool rbt_delete(RBTreeNode** pRoot, const char* key);

#ifdef __cplusplus
}
#endif

#endif // COMMON_H