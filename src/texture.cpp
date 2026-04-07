#include "texture.h"

Texture::Texture(const char *fileName, GLenum texType, GLenum slot, GLenum format, GLenum pixeltype)
{
    type = texType;
    
    int widthImg, heightImg, numColCh;
    // Flip image so that it appears right side up
    stbi_set_flip_vertically_on_load(true);
    // Read image from file and store as bytes
    unsigned char* bytes = stbi_load(fileName, &widthImg, &heightImg, &numColCh, 0);

    glGenTextures(1, &ID);
    glActiveTexture(slot);
    glBindTexture(texType, ID);
    
    glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixeltype, bytes);
    glGenerateMipmap(texType);

    // Image data no longer needed; free it
    stbi_image_free(bytes);
    // Unbind image texture
    glBindTexture(texType, 0);
}

void Texture::texUnit(Shader& shader, const char *uniform, GLuint unit)
{
    GLuint texUni = glGetUniformLocation(shader.ID, uniform);
    shader.Activate();
    glUniform1i(texUni, unit);
}

void Texture::Bind()
{
    glBindTexture(type, ID);
}

void Texture::Unbind()
{
    glBindTexture(type, 0);
}

void Texture::Delete()
{
    glDeleteTextures(1, &ID);
}
