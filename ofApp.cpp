#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openframeworks");

	ofBackground(39);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	for (int i = 0; i < 12; i++) {

		this->param_list.push_back(glm::vec3(100 + i * 0.01, 200 + i * 0.01, 300 + i * 0.01));
	}
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWindowSize() * 0.5);

	auto len = 25;
	ofColor color;

	auto head_size = 7;
	for (int k = 0; k < this->param_list.size(); k++) {

		ofPushMatrix();

		ofMesh mesh;
		vector<glm::vec3> right, left;

		glm::vec3 last_location;
		float last_theta, last_radius;

		for (int i = 0; i < len; i++) {

			auto noise_deg = ofMap(ofNoise(this->param_list[k].x, (ofGetFrameNum() + i) * 0.003), 0, 1, -360, 360);
			auto noise_radius = ofMap(ofNoise(this->param_list[k].y, (ofGetFrameNum() + i) * 0.0085), 0, 1, -320, 320);
			auto next_noise_deg = ofMap(ofNoise(this->param_list[k].x, (ofGetFrameNum() + i + 1) * 0.003), 0, 1, -360, 360);
			auto next_noise_radius = ofMap(ofNoise(this->param_list[k].y, (ofGetFrameNum() + i + 1) * 0.0085), 0, 1, -320, 320);

			auto location = glm::vec3(noise_radius * cos(noise_deg * DEG_TO_RAD), noise_radius * sin(noise_deg * DEG_TO_RAD), 0);
			auto next = glm::vec3(next_noise_radius * cos(next_noise_deg * DEG_TO_RAD), next_noise_radius * sin(next_noise_deg * DEG_TO_RAD), 0);

			auto direction = next - location;
			auto theta = atan2(direction.y, direction.x);

			right.push_back(location + glm::vec3(ofMap(i, 0, len, 0, head_size) * cos(theta + PI * 0.5), ofMap(i, 0, 25, 0, head_size) * sin(theta + PI * 0.5), 0));
			left.push_back(location + glm::vec3(ofMap(i, 0, len, 0, head_size) * cos(theta - PI * 0.5), ofMap(i, 0, 25, 0, head_size) * sin(theta - PI * 0.5), 0));

			last_location = location;
			last_theta = theta;
			last_radius = noise_radius;
		}

		for (int i = 0; i < right.size(); i++) {

			color.setHsb(ofMap(k, 0, this->param_list.size(), 0, 255), 150, 255);

			mesh.addVertex(left[i]);
			mesh.addVertex(right[i]);

			mesh.addColor(color);
			mesh.addColor(color);
		}

		for (int i = 0; i < mesh.getNumVertices() - 2; i += 2) {

			mesh.addIndex(i + 0); mesh.addIndex(i + 1); mesh.addIndex(i + 3);
			mesh.addIndex(i + 0); mesh.addIndex(i + 2); mesh.addIndex(i + 3);
		}

		auto tmp_head_size = ofMap(mesh.getNumVertices() - 2, 0, mesh.getNumVertices(), 0, head_size);

		mesh.addVertex(last_location);
		mesh.addColor(color);
		int index = mesh.getNumVertices();
		for (auto theta = last_theta - PI * 0.5; theta <= last_theta + PI * 0.5; theta += PI / 20) {

			mesh.addVertex(last_location + glm::vec3(tmp_head_size * cos(theta), tmp_head_size * sin(theta), 0));
			mesh.addColor(color);
		}

		for (int i = index; i < mesh.getNumVertices() - 1; i++) {

			mesh.addIndex(index); mesh.addIndex(i + 0); mesh.addIndex(i + 1);
		}

		if (abs(last_radius) < 2) { this->param_list[k].z += 0.1; }


		int span = 4;
		int deg_span = 360 / span;
		for (int i = 0; i < span; i++) {

			ofRotate(deg_span);
			mesh.draw();
		}

		ofPopMatrix();
	}

	/*
	int start = 200;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
glm::vec2 ofApp::make_point(int len, int param) {

	param = param % 100;
	if (param < 25) {

		return glm::vec2(ofMap(param, 0, 25, -len * 0.5, len * 0.5), -len * 0.5);
	}
	else if (param < 50) {

		return glm::vec2(len * 0.5, ofMap(param, 25, 50, -len * 0.5, len * 0.5));
	}
	else if (param < 75) {

		return glm::vec2(ofMap(param, 50, 75, len * 0.5, -len * 0.5), len * 0.5);
	}
	else {

		return glm::vec2(-len * 0.5, ofMap(param, 75, 100, len * 0.5, -len * 0.5));
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}