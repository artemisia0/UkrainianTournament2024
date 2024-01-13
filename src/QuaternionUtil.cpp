#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;

#include <QuaternionUtil.hpp>


quat RotationBetweenVectors(vec3 start, vec3 dest){
	start = normalize(start);
	dest = normalize(dest);
	
	float cosTheta = dot(start, dest);
	vec3 rotationAxis;
	
	if (cosTheta < -1 + 0.001f){
		rotationAxis = cross(vec3(0.0f, 0.0f, 1.0f), start);
		if (length2(rotationAxis) < 0.01 )
			rotationAxis = cross(vec3(1.0f, 0.0f, 0.0f), start);
		
		rotationAxis = normalize(rotationAxis);
		return angleAxis(glm::radians(180.0f), rotationAxis);
	}

	rotationAxis = cross(start, dest);

	float s = sqrt( (1+cosTheta)*2 );
	float invs = 1 / s;

	return quat(
		s * 0.5f, 
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
	);


}

quat LookAt(vec3 direction, vec3 desiredUp){

	if (length2(direction) < 0.0001f )
		return quat();

	vec3 right = cross(direction, desiredUp);
	desiredUp = cross(right, direction);
	
	quat rot1 = RotationBetweenVectors(vec3(0.0f, 0.0f, 1.0f), direction);
	vec3 newUp = rot1 * vec3(0.0f, 1.0f, 0.0f);
	quat rot2 = RotationBetweenVectors(newUp, desiredUp);
	
	return rot2 * rot1;
}

quat RotateTowards(quat q1, quat q2, float maxAngle){
	
	if( maxAngle < 0.001f ){
		return q1;
	}
	
	float cosTheta = dot(q1, q2);
	
	if(cosTheta > 0.9999f){
		return q2;
	}
	
	if (cosTheta < 0){
		q1 = q1*-1.0f;
		cosTheta *= -1.0f;
	}
	
	float angle = acos(cosTheta);

	if (angle < maxAngle){
		return q2;
	}

	float t = maxAngle / angle;
	angle = maxAngle;
	
	quat res = (sin((1.0f - t) * angle) * q1
				+ sin(t * angle) * q2) / sin(angle);
	res = normalize(res);
	return res;
	
}
