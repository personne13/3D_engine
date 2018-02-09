#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include "shader.h"

int get_index_shader(Shader *shader);
char* loadSource(char *filename);
void print_log_and_exit_if_error(Program *prog);

int SHADER_init_shaders(){
  return 1;
}

Shader SHADER_get_new_shader(GLenum type){
  Shader sh;

  sh.id = glCreateShader(type);
  sh.type = type;

  return sh;
}

int SHADER_load_shader(Shader *shader, char *path){
  char *src = loadSource(path);
  int logsize;
  char *log;
  GLint compile_status = GL_TRUE;

  glShaderSource(shader->id, 1, (const GLchar**)&src, NULL);
  glCompileShader(shader->id);
  glGetShaderiv(shader->id, GL_COMPILE_STATUS, &compile_status);
  if(compile_status != GL_TRUE){
      glGetShaderiv(shader->id, GL_INFO_LOG_LENGTH, &logsize);
      log = malloc(logsize + 1);
      if(log == NULL){
          fprintf(stderr, "Error allocating memory\n");
          exit(EXIT_FAILURE);
      }
      memset(log, '\0', logsize + 1);

      glGetShaderInfoLog(shader->id, logsize, &logsize, log);
      fprintf(stderr, "Error compilating '%s' :\n%s", path, log);

      free(log);
      exit(EXIT_FAILURE);
  }
  free(src);
  return 1;
}

void SHADER_delete_shader(Shader *shader){
  if(shader->id)
    glDeleteShader(shader->id);

  shader->id = 0;
}

char* loadSource(char *path){
  char *src = NULL;
  FILE *fp = NULL;
  long size;

  fp = fopen(path, "r");
  if(fp == NULL){
    fprintf(stderr, "Error opening file (%s)\n", path);
    exit(EXIT_FAILURE);
  }

  fseek(fp, 0, SEEK_END);
  size = ftell(fp);

  rewind(fp);

  src = malloc(size+1);
  if(src == NULL){
    fclose(fp);
    fprintf(stderr, "Error allocating memory (%ld bytes)\n", size);
    exit(EXIT_FAILURE);
  }

  for(long i = 0; i < size; i++)
    src[i] = fgetc(fp);

  src[size] = '\0';
  fclose(fp);

  return src;
}

Program SHADER_get_new_program(){
  Program prog;

  prog.id = glCreateProgram();
  prog.shader[0].id = 0;
  prog.shader[1].id = 0;

  return prog;
}

int get_index_shader(Shader *shader){
  return shader->type == GL_FRAGMENT_SHADER;
}

void print_log_and_exit_if_error(Program *prog){
  GLint link_status = GL_TRUE;
  int logsize;
  char *log;

  glGetProgramiv(prog->id, GL_COMPILE_STATUS, &link_status);
  if(link_status != GL_TRUE){
    glGetProgramiv(prog->id, GL_INFO_LOG_LENGTH, &logsize);
    log = malloc(logsize + 1);
    if(log == NULL){
      fprintf(stderr, "Error allocating memory\n");
      exit(EXIT_FAILURE);
    }
    memset(log, '\0', logsize + 1);

    glGetProgramInfoLog(prog->id, logsize, &logsize, log);
    fprintf(stderr, "Error linking program :\n%s", log);

    free(log);
    exit(EXIT_FAILURE);
  }
}

int SHADER_attach_shader_to_program(Program *prog, Shader *shader){
  int index = 0;

  if(!shader->id){
    fprintf(stderr, "Error : shader not loaded\n");
    return 0;
  }

  index = get_index_shader(shader);

  if(prog->shader[index].id){
    fprintf(stderr, "Error : program already has that type of shader\n");
    return 0;
  }

  prog->shader[index] = *shader;
  glAttachShader(prog->id, shader->id);
  glLinkProgram(prog->id);
  print_log_and_exit_if_error(prog);

  return 1;
}

int SHADER_detach_shader_from_program(Program *prog, Shader *shader){
  if(!shader->id){
    fprintf(stderr, "Error : shader not loaded\n");
    return 0;
  }

  int index = get_index_shader(shader);

  if(!prog->shader[index].id){
    fprintf(stderr, "Error : shader not attached to the program\n");
    return 0;
  }
  prog->shader[index].id = 0;
  glDetachShader(prog->id, shader->id);
  glLinkProgram(prog->id);
  print_log_and_exit_if_error(prog);

  return 1;
}

void SHADER_delete_program(Program *prog){
  glDeleteProgram(prog->id);
}

void SHADER_delete_program_and_attached_shader(Program *prog){
  if(prog->shader[0].id)
    SHADER_delete_shader(&prog->shader[0]);

  if(&prog->shader[1].id)
    SHADER_delete_shader(&prog->shader[1]);

  SHADER_delete_program(prog);
}

void SHADER_use_program(Program *prog){
  glUseProgram(prog->id);
}


void SHADER_disable_shaders(){
  glUseProgram(0);
}
