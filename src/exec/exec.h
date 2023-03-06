#ifndef EXEC_H
#define EXEC_H

#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../parser/parser3.h"

int exec_command_list(struct ast_node_command_list *node);

#endif /* EXEC_H */
