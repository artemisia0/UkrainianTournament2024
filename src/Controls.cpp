#include <gl.hpp>
#include <Context.hpp>
#include <Controls.hpp>
#include <Util.hpp>
#include <Settings.hpp>


static glm::mat4 ViewMatrix;
static glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}

#define position context->settings->cameraPosition
#define horizontalAngle context->settings->cameraAngleInHP
#define verticalAngle context->settings->cameraAngleInVP
#define initialFoV context->settings->cameraFieldOfView
#define speed context->settings->cameraSpeed
#define mouseSpeed context->settings->mouseSpeed

void computeMatricesFromInputs(bool controlEnabled,
							   Context* context,
							   double dt) {
	double winSizeX = context->settings->windowSize.x;
	double winSizeY = context->settings->windowSize.y;
	float deltaTime = dt;
	double xpos, ypos;
	glfwGetCursorPos(context->window, &xpos, &ypos);
	
    if (controlEnabled)
    {
		glfwSetCursorPos(context->window, winSizeX/2., winSizeY/2.);

		if (std::abs(winSizeX/2. - xpos) > 100
			or std::abs(winSizeY/2.f - ypos) > 100) {
			return;
		}
		horizontalAngle += mouseSpeed * float(winSizeX/2. - xpos );
		verticalAngle   += mouseSpeed * float(winSizeY/2. - ypos );
    }

	if (verticalAngle > 3.14/2.0) {
		verticalAngle = 3.14/2.0;
	}
	if (verticalAngle < -3.14/2.0) {
		verticalAngle = -3.14/2.0;
	}

	glm::vec3 direction = polarToCartesian(horizontalAngle, verticalAngle);
	context->settings->cameraDirection = direction;
	
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
		);
	
	glm::vec3 up = glm::cross( right, direction );
	
    if (controlEnabled)
    {
		if (glfwGetKey( context->window, GLFW_KEY_W ) == GLFW_PRESS){
			position += direction * deltaTime * speed;
		}
		if (glfwGetKey( context->window, GLFW_KEY_S ) == GLFW_PRESS){
			position -= direction * deltaTime * speed;
		}
		if (glfwGetKey( context->window, GLFW_KEY_D ) == GLFW_PRESS){
			position += right * deltaTime * speed;
		}
		if (glfwGetKey( context->window, GLFW_KEY_A ) == GLFW_PRESS){
			position -= right * deltaTime * speed;
		}
    }
	
	float FoV = initialFoV;
	
	ProjectionMatrix = glm::perspective(glm::radians(FoV),
										float(winSizeX/winSizeY),
										context->settings->nearPlane,
										context->settings->farPlane);
	
	ViewMatrix = glm::lookAt(
		position,       
		position+direction,
		up     
		);
}

#undef position
#undef horizontalAngle
#undef verticalAngle
#undef initialFoV
#undef speed
#undef mouseSpeed
