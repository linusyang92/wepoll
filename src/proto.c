#include <stdlib.h>

#include "error.h"
#include "proto.h"
#include "tree.h"
#include "win.h"

WEPOLL_INTERNAL proto_node_t* find_proto_node(proto_info_t* proto_info, DWORD catalog_id) {
    if (proto_info == NULL)
      return_error(NULL);

    proto_node_t* node = safe_container_of(
      tree_find(&proto_info->tree, catalog_id), proto_node_t, tree_node);

    if (node == NULL)
        return_error(NULL, ERROR_NOT_FOUND);
    return node;
}

WEPOLL_INTERNAL int proto_info_init(proto_info_t* proto_info) {
  DWORD buffer_size = 0;
  int i, r;
  WSAPROTOCOL_INFOW* prots;

  if (proto_info == NULL)
    return_error(-1);

  tree_init(&proto_info->tree);

  r = WSAEnumProtocolsW(NULL, NULL, &buffer_size);
  if (r == SOCKET_ERROR && WSAGetLastError() != WSAENOBUFS)
    return_error(-1);

  prots = malloc(buffer_size);
  if (prots == NULL)
    return_error(-1, ERROR_NOT_ENOUGH_MEMORY);
  memset(prots, 0, buffer_size);

  r = WSAEnumProtocolsW(NULL, prots, &buffer_size);
  if (r == SOCKET_ERROR) {
    free(prots);
    return_error(-1);
  }

  proto_info->allocator = malloc(sizeof(proto_node_t) * r);
  if (proto_info->allocator == NULL) {
    free(prots);
    return_error(-1, ERROR_NOT_ENOUGH_MEMORY);
  }

  for (i = 0; i < r; i++) {
    WSAPROTOCOL_INFOW* p = &prots[i];
    proto_node_t* proto_node = &proto_info->allocator[i];
    proto_node->info = *p;
    tree_node_init(&proto_node->tree_node);
    tree_add(&proto_info->tree, &proto_node->tree_node, p->dwCatalogEntryId);
  }

  free(prots);
  return 0;
}

WEPOLL_INTERNAL int proto_info_delete(proto_info_t* proto_info) {
  if (proto_info == NULL)
    return_error(-1);

  if (proto_info->allocator != NULL) {
    free(proto_info->allocator);
    proto_info->allocator = NULL;
    tree_init(&proto_info->tree);
  }
  return 0;
}
