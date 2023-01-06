project(
  '42sh',
  'c',
  version : '1.1.0',
  default_options : ['warning_level=3', 'c_std=c99', 'c_args=-D_POSIX_C_SOURCE=200809L -Wvla -Werror -Wall -Wextra -pedantic']
)

src = [
  'src/main.c',
  'src/exec/exec.c',
  'src/io/stream.c',
  'src/parser/parser.c',
  'src/lexer/lexer.c',
  'src/lexer/token.c'
]

project_dependencies = [
]

build_args = [
]

project_target = executable('42sh', src)
test('basic', project_target)
