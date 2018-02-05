#ifndef SHADER_H
#define SHADER_H

  #define NB_SHADERS 2

  typedef struct Shader Shader;
  struct Shader{
    GLuint id;
    GLuint type;
  };

  typedef struct Program Program;
  struct Program{
    GLuint id;
    Shader shader[NB_SHADERS];
  };

  int SHADER_init_shaders();
  //type is either FRAGMENT_SHADER or VERTEX_SHADER
  Shader SHADER_get_new_shader(GLenum type);
  int SHADER_load_shader(Shader *shader, char *path);
  void SHADER_delete_shader(Shader *shader);

  Program SHADER_get_new_program();
  int SHADER_attach_shader_to_program(Program *prog, Shader *shader);
  int SHADER_detach_shader_from_program(Program *prog, Shader *shader);
  void SHADER_delete_program(Program *prog);
  void SHADER_delete_program_and_attached_shader(Program *prog);
  void SHADER_use_program(Program *prog);

#endif
