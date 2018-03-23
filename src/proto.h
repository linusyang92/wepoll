#ifndef WEPOLL_PROTO_H_
#define WEPOLL_PROTO_H_

#include "tree.h"
#include "win.h"

typedef struct proto_node {
  tree_node_t tree_node;
  WSAPROTOCOL_INFOW info;
} proto_node_t;

typedef struct proto_info {
    tree_t tree;
    proto_node_t* allocator;
} proto_info_t;

WEPOLL_INTERNAL proto_node_t* find_proto_node(proto_info_t* proto_info, DWORD catalog_id);
WEPOLL_INTERNAL int proto_info_init(proto_info_t* proto_info);
WEPOLL_INTERNAL int proto_info_delete(proto_info_t* proto_info);

#endif /* WEPOLL_PROTO_H_ */
