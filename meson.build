project(
  '42sh',
  'c',
  version : '1.1.0',
  default_options : ['warning_level=3', 'c_std=c99', 'c_args=-D_POSIX_C_SOURCE=200809L -Wvla -Werror -Wall -Wextra -pedantic -fsanitize-recover -g']
)

src = [
  'src/main.c',
  'src/exec/exec.c',
  'src/io/stream.c',
  'src/parser/parser.c',
  'src/parser/simple_command.c',
  'src/parser/command_list.c',
  'src/lexer/lexer.c',
  'src/lexer/token.c',
  'src/builtins/echo.c',
  'src/builtins/cd.c',
  'src/builtins/exit.c',
  'src/parser/if_command.c',
  'src/parser/compound_list.c',
  'src/parser/command.c',
  'src/parser/pipe.c',
  'src/parser/while.c',
  'src/parser/until.c',
  'src/lexer/lexer_utils.c',
  'src/parser/and_or.c',
  'src/parser/for.c'
]

project_dependencies = [
]

build_args = [
]

project_target = executable('42sh', src)
test('basic', project_target)
