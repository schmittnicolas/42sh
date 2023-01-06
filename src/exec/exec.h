#ifndef EXEC_H
#define EXEC_H

#include <sys/wait.h>
#include <unistd.h>
#include "../parser/parser.h"

int exec_simple_command(struct ast_node_simple_command *node);

#endif /* EXEC_H */