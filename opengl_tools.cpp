#include "opengl_tools.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

ToolsC::ToolsC(std::string BASE_PATH_in)
{
	BASE_PATH = BASE_PATH_in;
}

void ToolsC::loadTextures()
{
    std::string texture_file_name;
	// texture 1
	texture_file_name = "/resources/container2.png";
	std::cout << "Loading " << texture_file_name << std::endl;
    glGenTextures(1, &(this->m_textures[0]));
    glBindTexture(GL_TEXTURE_2D, this->m_textures[0]);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load(std::string(BASE_PATH + texture_file_name).c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture " << texture_file_name << " loaded OK." << std::endl;
    }
    else
    {
		std::cout << "Failed to load texture "<< texture_file_name << std::endl;
    }
    stbi_image_free(data);

	// texture 1 specular
	// ---------
	texture_file_name = "/resources/container2_specular.png";
	std::cout << "Loading " << texture_file_name << std::endl;
	glGenTextures(1, &(this->m_textures[1]));
	glBindTexture(GL_TEXTURE_2D, this->m_textures[1]);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	data = stbi_load(std::string(BASE_PATH + texture_file_name).c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture " << texture_file_name << " loaded OK." << std::endl;
	}
	else
	{
		std::cout << "Failed to load texture " << texture_file_name << std::endl;
	}
	stbi_image_free(data);

    // texture 2
    // ---------
	texture_file_name = "/resources/checker_purple.jpg";
	std::cout << "Loading " << texture_file_name << std::endl;
    glGenTextures(1, &(this->m_textures[2]));
    glBindTexture(GL_TEXTURE_2D, this->m_textures[2]);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load(std::string(BASE_PATH + texture_file_name).c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture " << texture_file_name << " loaded OK." <<std::endl;
    }
    else
    {
		std::cout << "Failed to load texture " << texture_file_name << std::endl;
    }
    stbi_image_free(data);

    // texture 3
    // ---------
	texture_file_name = "/resources/checker_purple.jpg";
	std::cout << "Loading " << texture_file_name << std::endl;
    glGenTextures(1, &(this->m_textures[3]));
    glBindTexture(GL_TEXTURE_2D, this->m_textures[3]);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load(std::string(BASE_PATH + texture_file_name).c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture " << texture_file_name << " loaded OK." << std::endl;
    }
    else
    {
		std::cout << "Failed to load texture " << texture_file_name << std::endl;
    }
    stbi_image_free(data);
}

//
// initializes VBOs and VBAs
//
void ToolsC::initRenderData()
{

    GLuint *VBOs_in = this->m_VBOs;

    GLuint *VAOs_in = this->m_VAOs;
    //
    // VAO and VBO for 2D texture that shows webcam input
    //
    float factor = 4.0f;
    glm::vec2 square_size(SCR_WIDTH/factor, SCR_HEIGHT/factor);
    GLfloat vertices[] = { 
        // Pos      // Tex
        0.0f,          0.0f,            0.0f, 0.0f,
        square_size.x, square_size.y,   1.0f, 1.0f,
        square_size.x,  0.0f,           1.0f, 0.0f, 
    
        0.0f,           0.0f,           0.0f, 0.0f,
        0.0f,           square_size.y,  0.0f, 1.0f,
        square_size.x,  square_size.y,  1.0f, 1.0f
    };

    glGenVertexArrays(1, &(VAOs_in[0]));
    glGenBuffers(1, &(VBOs_in[0]));
    
    glBindBuffer(GL_ARRAY_BUFFER, (VBOs_in[0]));
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAOs_in[0]);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  
    glBindVertexArray(0);

    //
    // VAO and VBO for 3D floor
    //
    GLfloat num_tiles = 8.0f;
    GLfloat vertices_floor[] = { 
        // 3 Pos                // 3 normals		  //2 tex
        -1.0f, -1.0f, 0.05f,	0.0f,  0.0f, -1.0f,   0.0f,               0.0f,
         1.0f, -1.0f, 0.05f,	0.0f,  0.0f, -1.0f,   1.0f * num_tiles,   0.0f,
         1.0f,  1.0f, 0.05f,	0.0f,  0.0f, -1.0f,	  1.0f * num_tiles,   1.0f * num_tiles,
         1.0f,  1.0f, 0.05f,	0.0f,  0.0f, -1.0f,	  1.0f * num_tiles,   1.0f * num_tiles,
        -1.0f,  1.0f, 0.05f,	0.0f,  0.0f, -1.0f,   0.0f,               1.0f * num_tiles,
        -1.0f, -1.0f, 0.05f,	0.0f,  0.0f, -1.0f,   0.0f,               0.0f,
    };
    glGenVertexArrays(1, &(VAOs_in[1]));
    glGenBuffers(1, &(VBOs_in[1]));
    
    glBindBuffer(GL_ARRAY_BUFFER, VBOs_in[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_floor), vertices_floor, GL_STATIC_DRAW);

    glBindVertexArray(VAOs_in[1]);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);  
    glBindVertexArray(0);   

    //
    // VAO and VBO for cube object
    //
    float vertices_cube[] = {
		 // 3 Pos                // 3 normals		  //2 tex
        -0.05f, -0.05f, -0.15f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
         0.05f, -0.05f, -0.15f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
         0.05f,  0.05f, -0.15f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
         0.05f,  0.05f, -0.15f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
        -0.05f,  0.05f, -0.15f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
        -0.05f, -0.05f, -0.15f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
	
        -0.05f, -0.05f,  0.05f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
         0.05f, -0.05f,  0.05f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
         0.05f,  0.05f,  0.05f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
         0.05f,  0.05f,  0.05f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
        -0.05f,  0.05f,  0.05f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
        -0.05f, -0.05f,  0.05f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
	
        -0.05f,  0.05f,  0.05f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        -0.05f,  0.05f, -0.15f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        -0.05f, -0.05f, -0.15f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.05f, -0.05f, -0.15f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.05f, -0.05f,  0.05f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
        -0.05f,  0.05f,  0.05f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
	
         0.05f,  0.05f,  0.05f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
         0.05f,  0.05f, -0.15f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
         0.05f, -0.05f, -0.15f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
         0.05f, -0.05f, -0.15f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
         0.05f, -0.05f,  0.05f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
         0.05f,  0.05f,  0.05f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
	
        -0.05f, -0.05f, -0.15f, 0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
         0.05f, -0.05f, -0.15f, 0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
         0.05f, -0.05f,  0.05f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
         0.05f, -0.05f,  0.05f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
        -0.05f, -0.05f,  0.05f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
        -0.05f, -0.05f, -0.15f, 0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
	
        -0.05f,  0.05f, -0.15f, 0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
         0.05f,  0.05f, -0.15f, 0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
         0.05f,  0.05f,  0.05f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
         0.05f,  0.05f,  0.05f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
        -0.05f,  0.05f,  0.05f, 0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
        -0.05f,  0.05f, -0.15f, 0.0f,  1.0f,  0.0f, 0.0f, 1.0f
    };
 	
    //unsigned int VBO, VAO;
    glGenVertexArrays(1, &(VAOs_in[2]));
    glGenBuffers(1, &(VBOs_in[2]));
	
    glBindVertexArray(VAOs_in[2]);
	
    glBindBuffer(GL_ARRAY_BUFFER, VBOs_in[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_cube), vertices_cube, GL_STATIC_DRAW);
	
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);  
    glBindVertexArray(0);   

	//
// VAO and VBO for cube object
//
	float vertices_cube2[] = {
		// 3 Pos                // 3 normals		  //2 tex
	   -0.05f, -0.05f, -0.05f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
		0.05f, -0.05f, -0.05f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
		0.05f,  0.05f, -0.05f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
		0.05f,  0.05f, -0.05f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
	   -0.05f,  0.05f, -0.05f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
	   -0.05f, -0.05f, -0.05f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

	   -0.05f, -0.05f,  0.05f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
		0.05f, -0.05f,  0.05f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
		0.05f,  0.05f,  0.05f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
		0.05f,  0.05f,  0.05f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
	   -0.05f,  0.05f,  0.05f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
	   -0.05f, -0.05f,  0.05f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

	   -0.05f,  0.05f,  0.05f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
	   -0.05f,  0.05f, -0.05f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
	   -0.05f, -0.05f, -0.05f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	   -0.05f, -0.05f, -0.05f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	   -0.05f, -0.05f,  0.05f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
	   -0.05f,  0.05f,  0.05f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

		0.05f,  0.05f,  0.05f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		0.05f,  0.05f, -0.05f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		0.05f, -0.05f, -0.05f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		0.05f, -0.05f, -0.05f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		0.05f, -0.05f,  0.05f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		0.05f,  0.05f,  0.05f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

	   -0.05f, -0.05f, -0.05f, 0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
		0.05f, -0.05f, -0.05f, 0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
		0.05f, -0.05f,  0.05f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		0.05f, -0.05f,  0.05f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
	   -0.05f, -0.05f,  0.05f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
	   -0.05f, -0.05f, -0.05f, 0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

	   -0.05f,  0.05f, -0.05f, 0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
		0.05f,  0.05f, -0.05f, 0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
		0.05f,  0.05f,  0.05f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		0.05f,  0.05f,  0.05f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
	   -0.05f,  0.05f,  0.05f, 0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
	   -0.05f,  0.05f, -0.05f, 0.0f,  1.0f,  0.0f, 0.0f, 1.0f
	};

	//unsigned int VBO, VAO;
	glGenVertexArrays(1, &(VAOs_in[3]));
	glGenBuffers(1, &(VBOs_in[3]));

	glBindVertexArray(VAOs_in[3]);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs_in[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_cube2), vertices_cube2, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
