// �E�B���h�E�֘A�̏���
#include "Window.h"

// �W�����C�u����
#include <vector>

//
// �V�F�[�_�I�u�W�F�N�g�̃R���p�C�����ʂ�\������
//
//   shader: �V�F�[�_�I�u�W�F�N�g��
//   str: �R���p�C���G���[�����������ꏊ������������
//   �߂�l: �R���p�C���ɐ������Ă����� GL_TRUE
//
static GLboolean printShaderInfoLog(GLuint shader, const char *str)
{
  // �R���p�C�����ʂ��擾����
  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) std::cerr << "Compile Error in " << str << std::endl;

  // �V�F�[�_�̃R���p�C�����̃��O�̒������擾����
  GLsizei bufSize;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);

  if (bufSize > 1)
  {
    // �V�F�[�_�̃R���p�C�����̃��O�̓��e���擾����
    std::vector<GLchar> infoLog(bufSize);
    GLsizei length;
    glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);
    std::cerr << &infoLog[0] << std::endl;
  }

  return static_cast<GLboolean>(status);
}

//
// �v���O�����I�u�W�F�N�g�̃����N���ʂ�\������
//
//   program: �v���O�����I�u�W�F�N�g��
//   �߂�l: �����N�ɐ������Ă����� GL_TRUE
//
static GLboolean printProgramInfoLog(GLuint program)
{
  // �����N���ʂ��擾����
  GLint status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) std::cerr << "Link Error." << std::endl;

  // �V�F�[�_�̃����N���̃��O�̒������擾����
  GLsizei bufSize;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);

  if (bufSize > 1)
  {
    // �V�F�[�_�̃����N���̃��O�̓��e���擾����
    std::vector<GLchar> infoLog(bufSize);
    GLsizei length;
    glGetProgramInfoLog(program, bufSize, &length, &infoLog[0]);
    std::cerr << &infoLog[0] << std::endl;
  }

  return static_cast<GLboolean>(status);
}

//
// �v���O�����I�u�W�F�N�g���쐬����
//
//   vsrc: �o�[�e�b�N�X�V�F�[�_�̃\�[�X�v���O�����̕�����
//   pv: �o�[�e�b�N�X�V�F�[�_�̃\�[�X�v���O�������� in �ϐ����̕�����
//   fsrc: �t���O�����g�V�F�[�_�̃\�[�X�v���O�����̕�����
//   fc: �t���O�����g�V�F�[�_�̃\�[�X�v���O�������� out �ϐ����̕�����
//   vert: �o�[�e�b�N�X�V�F�[�_�̃R���p�C�����̃��b�Z�[�W�ɒǉ����镶����
//   frag: �t���O�����g�V�F�[�_�̃R���p�C�����̃��b�Z�[�W�ɒǉ����镶����
//   �߂�l: �v���O�����I�u�W�F�N�g��
//
static GLuint createProgram(const char *vsrc, const char *pv, const char *fsrc, const char *fc,
  const char *vert = "vertex shader", const char *frag = "fragment shader")
{
  // ��̃v���O�����I�u�W�F�N�g���쐬����
  const GLuint program(glCreateProgram());

  if (vsrc != NULL)
  {
    // �o�[�e�b�N�X�V�F�[�_�̃V�F�[�_�I�u�W�F�N�g���쐬����
    const GLuint vobj(glCreateShader(GL_VERTEX_SHADER));
    glShaderSource(vobj, 1, &vsrc, NULL);
    glCompileShader(vobj);

    // �o�[�e�b�N�X�V�F�[�_�̃V�F�[�_�I�u�W�F�N�g���v���O�����I�u�W�F�N�g�ɑg�ݍ���
    if (printShaderInfoLog(vobj, vert))
      glAttachShader(program, vobj);
    glDeleteShader(vobj);
  }

  if (fsrc != NULL)
  {
    // �t���O�����g�V�F�[�_�̃V�F�[�_�I�u�W�F�N�g���쐬����
    const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
    glShaderSource(fobj, 1, &fsrc, NULL);
    glCompileShader(fobj);

    // �t���O�����g�V�F�[�_�̃V�F�[�_�I�u�W�F�N�g���v���O�����I�u�W�F�N�g�ɑg�ݍ���
    if (printShaderInfoLog(fobj, vert))
      glAttachShader(program, fobj);
    glDeleteShader(fobj);
  }

  // �v���O�����I�u�W�F�N�g�������N����
  glBindAttribLocation(program, 0, pv);
  glBindFragDataLocation(program, 0, fc);
  glLinkProgram(program);

  // �쐬�����v���O�����I�u�W�F�N�g��Ԃ�
  if (printProgramInfoLog(program))
    return program;

  // �v���O�����I�u�W�F�N�g���쐬�ł��Ȃ���� 0 ��Ԃ�
  glDeleteProgram(program);
  return 0;
}

//
// ���_�z��I�u�W�F�N�g�̍쐬
//
//   vertices: ���_��
//   position: ���_�̓񎟌��ʒu (GLfloat[2] �̔z��)
//   lines: ������
//   index: �����̒��_�C���f�b�N�X
//   �߂�l: �쐬���ꂽ���_�z��I�u�W�F�N�g��
//
static GLuint createObject(GLuint vertices, const GLfloat (*position)[2], GLuint lines, const GLuint *index)
{
  // ���_�z��I�u�W�F�N�g
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // ���_�o�b�t�@�I�u�W�F�N�g
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat[2]) * vertices, position, GL_STATIC_DRAW);

  // �C���f�b�N�X�o�b�t�@�I�u�W�F�N�g
  GLuint ibo;
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (GLuint) * lines, index, GL_STATIC_DRAW);

  // ��������Ă��钸�_�o�b�t�@�I�u�W�F�N�g�� in �ϐ�����Q�Ƃł���悤�ɂ���
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  // ���_�z��I�u�W�F�N�g�̌���������������ɒ��_�o�b�t�@�I�u�W�F�N�g�ƃC���f�b�N�X�o�b�t�@�I�u�W�F�N�g�̌�������������
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  return vao;
}

//
// ���C���v���O����
//
int main()
{
  // �E�B���h�E���쐬����
  Window window("ggsample02");

  // �w�i�F���w�肷��
  glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

  // �o�[�e�b�N�X�V�F�[�_�̃\�[�X�v���O����
  static const GLchar vsrc[] =
    "#version 150 core\n"
    "in vec4 pv;\n"
    "void main(void)\n"
    "{\n"
    "  gl_Position = pv;\n"
    "}\n";

  // �t���O�����g�V�F�[�_�̃\�[�X�v���O����
  static const GLchar fsrc[] =
    "#version 150 core\n"
    "out vec4 fc;\n"
    "void main(void)\n"
    "{\n"
    "  fc = vec4(1.0, 0.0, 0.0, 1.0);\n"
    "}\n";

  // �v���O�����I�u�W�F�N�g�̍쐬
  const GLuint program(createProgram(vsrc, "pv", fsrc, "fc"));

  // ���_����
  static const GLfloat position[][2] =
  {
    { -0.5f, -0.5f },
    {  0.5f, -0.5f },
    {  0.5f,  0.5f },
    { -0.5f,  0.5f }
  };
  static const int vertices(sizeof position / sizeof position[0]);

  // ���_�C���f�b�N�X
  static const GLuint index[] =
  {
    0, 2, 1, 3
  };
  static const GLuint lines(sizeof index / sizeof index[0]);

  // ���_�z��I�u�W�F�N�g�̍쐬
  const GLuint vao(createObject(vertices, position, lines, index));

  // �E�B���h�E���J���Ă���ԌJ��Ԃ�
  while (window.shouldClose() == GL_FALSE)
  {
    // �E�B���h�E����������
    glClear(GL_COLOR_BUFFER_BIT);

    // �V�F�[�_�v���O�����̎g�p�J�n
    glUseProgram(program);

    // �`��Ɏg�����_�z��I�u�W�F�N�g�̎w��
    glBindVertexArray(vao);

    // �}�`�̕`��
    glDrawElements(GL_LINE_LOOP, lines, GL_UNSIGNED_INT, 0);

    // ���_�z��I�u�W�F�N�g�̎w�����
    glBindVertexArray(0);

    // �V�F�[�_�v���O�����̎g�p�I��
    glUseProgram(0);

    // �J���[�o�b�t�@�����ւ��ăC�x���g�����o��
    window.swapBuffers();
  }
}
