#include "config.h"
#include "triangle_mesh.h"
#include "material.h"
#include "linear_algebros.h"



unsigned int make_module(const std::string& filepath, unsigned int module_type);

unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath) {

	//To store all the shader modules
	std::vector<unsigned int> modules;

	//Add a vertex shader module
	modules.push_back(make_module(vertex_filepath, GL_VERTEX_SHADER));

	//Add a fragment shader module
	modules.push_back(make_module(fragment_filepath, GL_FRAGMENT_SHADER));

	//Attach all the modules then link the program
	unsigned int shader = glCreateProgram();
	for (unsigned int shaderModule : modules) {
		glAttachShader(shader, shaderModule);
	}
	glLinkProgram(shader);

	//Check if the linking worked
	int success;
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success) {
		char errorLog[1024];
		glGetProgramInfoLog(shader, 1024, NULL, errorLog);
		std::cout << "Shader linking error:\n" << errorLog << '\n';
	}

	//Modules are now unneeded and can be freed
	for (unsigned int shaderModule : modules) {
		glDeleteShader(shaderModule);
	}

	return shader;

}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}




unsigned int make_module(const std::string& filepath, unsigned int module_type) {
	
	std::ifstream file;
	std::stringstream bufferedLines;
	std::string line;

	file.open(filepath);
	while (std::getline(file, line)) {
		//std::cout << line << std::endl;
		bufferedLines << line << '\n';
	}
	std::string shaderSource = bufferedLines.str();
	const char* shaderSrc = shaderSource.c_str();
	bufferedLines.str("");
	file.close();

	unsigned int shaderModule = glCreateShader(module_type);
	glShaderSource(shaderModule, 1, &shaderSrc, NULL);
	glCompileShader(shaderModule);

	int success;
	glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
	if (!success) {
		char errorLog[1024];
		glGetShaderInfoLog(shaderModule, 1024, NULL, errorLog);
		std::cout << "Shader Module compilation error:\n" << errorLog << std::endl;
	}

	return shaderModule;
}








int main() {
    std::cout << "Hello World" << std::endl;
    std::cout << "________" << std::endl;
    std::cout << "We start glfw below..." << std::endl;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1; 

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 960, "Hello World", NULL, NULL);
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }



    /* FInd and load OpenGL defintions*/
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Couldn't load opengl (glad didn't initialize)" << std::endl;
        glfwTerminate();
        return -1;

    }

    /* Change background color */
    glClearColor(0.75f, 0.5f, 0.15f, 1.0f);
    // Set the rendering region to the actual screen size
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);


    TriangleMesh* triangle = new TriangleMesh();
    Material* material = new Material("../img/pydatastructures.jpg");
    Material* mask = new Material("../img/mask.jpg");

    unsigned int shader = make_shader(
        "../src/shaders/vertex.txt",
        "../src/shaders/fragment.txt"
    );
    //set texture units
    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, "material"), 0);
    glUniform1i(glGetUniformLocation(shader, "mask"), 1);

    
    glm::vec3 quad_position = {0.1f, -0.2f, 0.0f};
    glm::vec3 camera_pos = {-0.4f, 0.5f, 0.6f};
    glm::vec3 camera_target = {0.0f, 0.0f, 0.5f};
    glm::vec3 up = {0.0f, 0.0f, 1.0f};
    // mat4 model = create_matrix_transformation(quad_position);    
    unsigned int model_location = glGetUniformLocation(shader, "model");
    unsigned int view_location = glGetUniformLocation(shader, "view");
    unsigned int proj_location = glGetUniformLocation(shader, "projection");



    glm::mat4 view = glm::lookAt(camera_pos, camera_target, up);
    glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 projection = glm::perspective(
        45.0f, (float)w/h, 0.1f, 10.0f 
    );
    glUniformMatrix4fv(proj_location, 1, GL_FALSE, glm::value_ptr(projection));



    // configure alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);






   
    glfwSetKeyCallback(window, key_callback);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Poll for and process events */
        glfwPollEvents();  // make sure it is first in the loop else 100% cpu-core usage
        
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);


        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, quad_position);
        model = glm::rotate(model, (float)glfwGetTime(), {0.0f, 0.0f, 1.0f});
        glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
        //older...
        // mat4 model = create_z_rotation(10 * glfwGetTime());
        //mat4 model = create_model_transform(quad_position, 10 * glfwGetTime());

        glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
        
        
        glUseProgram(shader);
        material->use(0);
        mask->use(1);
        triangle->draw();



        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        
    }
    glDeleteProgram(shader);
    delete triangle;
    delete material;
    delete mask;
    glfwDestroyWindow(window);

    glfwTerminate();
    return 0;
}