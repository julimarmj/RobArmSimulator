#include "kinematics.h"

Kinematic::Kinematic() :m_CountJoint{ 0 }, EofPos{ 0 }
{	
	std::vector<DH_Var> HD;
	HD.push_back({ 0.5f, 0.0f, 0.0f, 0.0f, -0.5f, 0.0f });
	HD.push_back({ 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f });
	HD.push_back({ 0.1057962f, -90.0f, 0.75f, 0.0f, 0.0f, 0.0f });
	HD.push_back({ 0.0f, 0.0f, -0.275f, 0.0f, 1.0f, 0.0f });
	HD.push_back({ 0.0f, 90.0f, 0.0f, 0.0f, 0.8f, 0.0f });
	HD.push_back({ 0.0f, -90.0f, 0.45f, 0.0f, -0.2f, 0.0f });
	HD.push_back({ 0.0f, 90.0f, 0.0f, 0.0f, 0.45f, 0.0f });
	HD.push_back({ 0.0f, 0.0f, 0.4f, 0.0f, 0.0f, 0.0f });

	std::vector<glm::fvec2> Limits;
	Limits.push_back({ -INFINITY, INFINITY });
	Limits.push_back({ -INFINITY, INFINITY });
	Limits.push_back({ -90.0f, 90.0f });
	Limits.push_back({ -120.0f, 120.0f });
	Limits.push_back({ -INFINITY, INFINITY });
	Limits.push_back({ -150.0f, 150.0f });
	Limits.push_back({ -INFINITY, INFINITY });
	Limits.push_back({ 0.0f, 0.0f });

	for (int i = 0; i < HD.size(); i++)
	{
		addJont(HD[i], Limits[i]);
		vEixo.push_back({ 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
						  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
						  0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
						  0.0f, 0.0f, 0.0f, 1.0f, 0.6f, 0.6f, 0.6f, 1.0f });
	}
	
}

Kinematic::~Kinematic()
{
}


void Kinematic::update()
{
	for (int i = 0; i < m_CountJoint; i++) {
		checkLimits(i);
		Joints[i].T = HDMatix(Joints[i].Dh);
		vEixo[i] = getGLJointMatix(i);
	}

	glm::mat4 Eof = getJointMatix(m_CountJoint-1);
	EofPos = { Eof[0].w, Eof[1].w ,Eof[2].w };

	static bool lkTPath = false;

	if (glfwJoystickPresent(GLFW_JOYSTICK_1)) {
		int axesCount;
		const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);

		if (axes[0] > 0.1)
			Joints[1].Dh.th -= pow(axes[0], 2) * m_Sence;
		else if(axes[0] < -0.1)
			Joints[1].Dh.th += pow(axes[0], 2) * m_Sence;

		if (axes[1] > 0.1)
			Joints[2].Dh.th += pow(axes[1], 2) * m_Sence;
		else if (axes[1] < -0.1)
			Joints[2].Dh.th -= pow(axes[1], 2) * m_Sence;

		if (axes[2] > 0.3)
			Joints[4].Dh.th += pow(axes[2], 2) * m_Sence;
		else if (axes[2] < -0.3)
			Joints[4].Dh.th -= pow(axes[2], 2) * m_Sence;

		if (axes[3] > 0.3)
			Joints[3].Dh.th += pow(axes[3], 2) * m_Sence;
		else if (axes[3] < -0.3)
			Joints[3].Dh.th -= pow(axes[3], 2) * m_Sence;

		if (axes[4] > -1)
			Joints[5].Dh.th += pow(axes[4], 2) * m_Sence;
		else if (axes[5] > -1)
			Joints[5].Dh.th -= pow(axes[4], 2) * m_Sence;

		int ButtonsCount;
		const unsigned char* Buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &ButtonsCount);
		static bool lkBt [sizeof(Buttons)];

		if (Buttons[11] == GLFW_PRESS)
			Joints[6].Dh.th += m_Sence;
		else if (Buttons[13] == GLFW_PRESS)
			Joints[6].Dh.th -= m_Sence;
		
		if (Buttons[7]) {

			for (int i = 0; i < Joints.size(); i++)
			{
				if (Joints[i].Dh.th > m_Sence)
					Joints[i].Dh.th -= m_Sence;
				else if (Joints[i].Dh.th < -m_Sence)
					Joints[i].Dh.th += m_Sence;
			}
		}

		if (Buttons[6] && !lkBt[0]) {
			m_Sence += 0.5;
			if (m_Sence > 2)
			{
				m_Sence = 0.5;
			}
			lkBt[0] = true;
		}else if (Buttons[6])
			lkBt[0] = false;

		if (Buttons[0])
		{
			//lkTPath = false;
			for (int i = 0; i < m_CountJoint - 1; i++)
			{
				Joints[i].AngBuffer.clear();
			}
			EofPosBuffer.clear();
			PosBufferIndex.clear();
		}

		if (Buttons[4] && !lkTPath && !lkBt[1])
		{
			lkTPath = true;
			lkBt[1] = true;
		}
		else if (Buttons[4] && lkTPath && !lkBt[1])
		{
			lkTPath = false;
			lkBt[1] = true;
		}
		else if (!Buttons[4])
			lkBt[1] = false;

		if (lkTPath)
		{
			for (int i = 0; i < m_CountJoint-1; i++)
			{
				Joints[i].AngBuffer.push_back(Joints[i].Dh.th);
			}

			EofPosBuffer.push_back({ EofPos[0] });
			EofPosBuffer.push_back({ EofPos[1] });
			EofPosBuffer.push_back({ EofPos[2] });
			EofPosBuffer.push_back({ 1 });
			EofPosBuffer.push_back({ 0 });
			EofPosBuffer.push_back({ 0 });
			EofPosBuffer.push_back({ 0 });
			EofPosBuffer.push_back({ 1 }); 
			PosBufferIndex.push_back(PosBufferIndex.size());
		}
		if (Buttons[5]) {
			for (int i = 0; i < m_CountJoint-1; i++)
			{
				if (!Joints[i].AngBuffer.empty()){
					Joints[i].Dh.th = Joints[i].AngBuffer.back();
					Joints[i].AngBuffer.emplace_back(Joints[i].AngBuffer.front());
					Joints[i].AngBuffer.erase(Joints[i].AngBuffer.begin());
				}
			}
		}
	}

}

//void Kinematic::EofMoveTo(glm::vec3 Pos)
//{
//	glm::vec3 valAbs = abs(Pos - EofPos);
//
//	if (valAbs[0] > 0.01 or valAbs[1] > 0.01 or valAbs[2] > 0.01) {
//		glm::vec3 IniG(0.01, 0.01, 0.01);
//
//		glm::vec3 _th(m_Dh[1].DH.th, m_Dh[2].DH.th, m_Dh[3].DH.th);
//		_th = glm::radians(_th);
//		_th += IniG;
//
//		for (int i = 0; i < 20 ; i++){
//			glm::vec3 th = _th;
//			glm::vec3 pf;
//
//			pf.x = 2*(m_Dh[4].b*cos(th.x)*(sin(th.y)*cos(th.z)-cos(th.y)*sin(th.z))+m_Dh[3].d * sin(th.x) + cos(th.x) * sin(th.y) + m_Dh[2].a * cos(th.x) + m_Dh[0].a) - EofPos.x - Pos.x;
//			pf.y = 2*(m_Dh[4].b*(-sin(th.y)* cos(th.z)+cos(th.y)*cos(th.z))+cos(th.y) + m_Dh[2].d + m_Dh[0].d) - EofPos.y - Pos.y;
//			pf.z = 2*(-m_Dh[4].b*sin(th.x)*(cos(th.y)* sin(th.z)+sin(th.y)*cos(th.z))+m_Dh[3].d * cos(th.x) - cos(th.x) * sin(th.y) - m_Dh[2].a * cos(th.x) + m_Dh[0].b) - EofPos.z - Pos.z;
//
//			glm::mat3 JAC = getJacobian(th.x, th.y, th.z);
//			glm::mat3 Dfinv = glm::inverse(JAC);
//
//			//glm::vec3 _th = th * Dfinv;
//			glm::vec3 axth;
//
//			axth.x = th.x - Dfinv[0][0] * pf.x + Dfinv[0][1] * pf.y + Dfinv[0][2] * pf.z;
//			axth.y = th.y - Dfinv[1][0] * pf.x + Dfinv[1][1] * pf.y + Dfinv[1][2] * pf.z;
//			axth.z = th.z - Dfinv[2][0] * pf.x + Dfinv[2][1] * pf.y + Dfinv[2][2] * pf.z;
//
//			glm::vec3 thAbs = abs(axth - _th);
//
//			if (thAbs.x < 5)
//				_th.x = axth.x;
//			else{
//				IniG.x = -IniG.x;
//				_th.x = IniG.x;
//			}
//			if (thAbs.y < 5)
//				_th.y = axth.y;
//			else {
//				IniG.y = -IniG.y;
//				_th.y = IniG.y;
//			}
//			if (thAbs.z < 5)
//				_th.z = axth.z;
//			else {
//				IniG.z = -IniG.z;
//				_th.z = IniG.z;
//			}
//		}
//		m_Dh[1].DH.th = glm::degrees(_th.x);
//		m_Dh[2].DH.th = glm::degrees(_th.y);
//		m_Dh[3].DH.th = glm::degrees(_th.z);
//	}
//}

//glm::mat3 Kinematic::getJacobian(float th1, float th2, float th3 )
//{
//	float Df1_Dth1 = 2*( m_Dh[4].b*sin(th1)*(sin(th2)*cos(th3)-cos(th2)*sin(th3))+m_Dh[3].d*cos(th1)-sin(th1)*sin(th2)- m_Dh[2].a*sin(th1));
//	float Df1_Dth2 = 2*(-m_Dh[4].b*cos(th1)*(cos(th2)*cos(th3)+sin(th2)*sin(th3))+cos(th1)*cos(th2));
//	float Df1_Dth3 = 2*( m_Dh[4].b*cos(th1)*(cos(th2)*cos(th3)+sin(th2)*sin(th3)));
//	float Df2_Dth1 = 0;
//	float Df2_Dth2 = 2*(-m_Dh[4].b*(cos(th2)*cos(th3)+cos(th2)*sin(th3))-sin(th2));
//	float Df2_Dth3 = 2*( m_Dh[4].b*(sin(th2)*sin(th3)-cos(th2)*sin(th3)));
//	float Df3_Dth1 = 2*(-m_Dh[4].b*cos(th1)*(cos(th2)* sin(th3)+sin(th2)*cos(th3))-m_Dh[3].d * sin(th1) - cos(th1) * sin(th2) - m_Dh[2].a * cos(th1));
//	float Df3_Dth2 = 2*(-m_Dh[4].b*sin(th1)*(cos(th2)*cos(th3)-sin(th2)*sin(th3))-sin(th1) * cos(th2));
//	float Df3_Dth3 = 2*(-m_Dh[4].b*sin(th1)*(cos(th2)* cos(th3)-sin(th2)*cos(th3)));
//
//	return glm::mat3(Df1_Dth1, Df1_Dth2, Df1_Dth3,
//					 Df2_Dth1, Df2_Dth2, Df2_Dth3,
//					 Df3_Dth3, Df3_Dth2, Df3_Dth3);
//}

void Kinematic::addJont(DH_Var& DH,glm::vec2 limits)
{
	glm::mat4 T = HDMatix(DH);
	Joints.push_back({ DH, T ,limits });
	Joints[m_CountJoint].DHM_I = getJointMatix(m_CountJoint);
	m_CountJoint++;
}

glm::mat4 Kinematic::getJointMatix(int Numjoint)
{
	std::vector<glm::mat4>vecAuxT(6,{1,0,0,0,
									 0,1,0,0,
									 0,0,1,0,
									 0,0,0,1});
	glm::mat4 AuxT{};
	for (int i = 0; i <= Numjoint; i++) {
		if (i == 0)
			AuxT = Joints[i].T;
		else
			AuxT = Joints[i].T * AuxT;
	}

	return AuxT;
}

glm::mat4 Kinematic::getAxes(int Numjoint)
{
	glm::mat4 AxisT(0);
	glm::mat4 HDM = getJointMatix(Numjoint-1);

	glm::mat4 T = Joints[Numjoint].T;
	T[0].w += 1;

	glm::mat4 AxisTx = T * HDM;

	return glm::mat4();
}

std::vector<float> Kinematic::getGLJointMatix(int i) 
{
	glm::mat4 AUX = getJointMatix(i);

	return std::vector<float>{AUX[0][3]+ AUX[0][0], AUX[1][3]+ AUX[0][1],AUX[2][3]+ AUX[0][2], 1, 1.0, 0.0, 0.0, 1,
							  AUX[0][3]+ AUX[1][0], AUX[1][3]+ AUX[1][1],AUX[2][3]+ AUX[1][2], 1, 0.0, 1.0, 0.0, 1,
							  AUX[0][3]+ AUX[2][0], AUX[1][3]+ AUX[2][1],AUX[2][3]+ AUX[2][2], 1, 0.0, 0.0, 1.0, 1,
										 AUX[0][3],            AUX[1][3],           AUX[2][3], 1, 0.6, 0.6, 0.6, 1};
}

void Kinematic::checkLimits(int& i)
{
	if (Joints[i].Dh.th < Joints[i].Limits.x)
		Joints[i].Dh.th = Joints[i].Limits.x;
	if (Joints[i].Dh.th > Joints[i].Limits.y)
		Joints[i].Dh.th = Joints[i].Limits.y;
}

glm::mat4 Kinematic::GetRotMatix(std::string axis, const float& O) {
	float _O = glm::radians(O);
	if (axis == "x") {
		return glm::mat4( 1,       0,       0, 0,
						  0, cos(_O), -sin(_O), 0,
						  0, sin(_O), cos(_O), 0,
						  0,       0,       0, 1 );
	}

	if (axis == "y") {
		return glm::mat4( cos(_O), 0, sin(_O), 0,
						        0, 1,       0, 0,
						  -sin(_O), 0, cos(_O), 0,
						        0, 0,       0, 1 );
	}

	if (axis == "z") {
		return glm::mat4( cos(_O), -sin(_O), 0, 0,
						  sin(_O), cos(_O), 0, 0,
						 	    0,       0, 1, 0,
						        0,       0, 0, 1 );
	}
}
glm::mat4 Kinematic::GetTransMatix(std::string axis, const float& T) {
	if (axis == "x") {
		return glm::mat4 ( 1, 0, 0, T,
						   0, 1, 0, 0,
						   0, 0, 1, 0,
						   0, 0, 0, 1 );
	}

	if (axis == "y") {
		return glm::mat4 ( 1, 0, 0, 0,
						   0, 1, 0, T,
						   0, 0, 1, 0,
						   0, 0, 0, 1 );
	}

	if (axis == "z") {
		return glm::mat4 ( 1, 0, 0, 0,
						   0, 1, 0, 0,
						   0, 0, 1, T,
						   0, 0, 0, 1 );
	}
}

glm::mat4 Kinematic::HDMatix(DH_Var& DH)
{
	glm::mat4 DHM = GetRotMatix("y", DH.th) * GetRotMatix("x", DH.alf);

	DHM[0].w = DH.a;
	DHM[1].w = DH.d;
	DHM[2].w = DH.b;

	return DHM;
}
