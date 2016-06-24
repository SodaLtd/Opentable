//
//  openTableClientSessionServer.cpp
//  openTableServer
//
//  Created by Jonathan Jones-morris on 04/02/2015.
//
//

#include "openTableClientSessionServer.h"
#include "ARMarker.hpp"
#include "ofApp.h"
#include "Poco/Net/HostEntry.h"
#include "Poco/Net/IPAddress.h"
#include "Poco/Net/NetworkInterface.h"
#include "Poco/Environment.h"
#include "Poco/Exception.h"
#include "Poco/StreamCopier.h"
#include "Poco/Net/DNS.h"
#include "Poco/Net/HostEntry.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/IPAddress.h"
#include "Poco/Net/NetworkInterface.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"

string getHostName()
{
	string nodeName = "UNKNOWN";

	try
	{
		nodeName = Poco::Net::DNS::hostName();
	}
	catch (const Poco::Net::HostNotFoundException& exc)
	{
		ofLogError("NetworkUtils::getNodeName") << exc.displayText();
		return nodeName;
	}
	catch (const Poco::Net::NoAddressFoundException& exc)
	{
		ofLogError("NetworkUtils::getNodeName") << exc.displayText();
		return nodeName;
	}
	catch (const Poco::Net::DNSException& exc)
	{
		ofLogError("NetworkUtils::getNodeName") << exc.displayText();
		return nodeName;
	}
	catch (const Poco::IOException& exc)
	{
		ofLogError("NetworkUtils::getNodeName") << exc.displayText();
		return nodeName;
	}
	catch (...)
	{
		ofLogError("NetworkUtils::getNodeName") << "Unknown Exception: " << nodeName;
		return nodeName;
	}

	return nodeName;
}
#ifdef TARGET_WIN32

map<string, NetInterface> getMyIPAddresses() {
	map<string, NetInterface> adresses;
	Poco::Net::NetworkInterface::NetworkInterfaceList network_interface_list = Poco::Net::NetworkInterface::list();
	for ( auto& network_interface : network_interface_list ) {
		if (!network_interface.supportsIPv6() && network_interface.supportsIPv4()) {
			string interface_name = network_interface.displayName();
			if (network_interface.isLoopback()) {
				printf("%s is loopback\n", interface_name.c_str());
			}
			else {
				if (!network_interface.isRunning()) {
					printf("%s is not running\n", interface_name.c_str());
				}
				if (!network_interface.isUp()) {
					printf("%s is not up\n", interface_name.c_str());
				}

				string ip_address = network_interface.address().toString();
				if (ip_address != "127.0.0.1" && ip_address != "0.0.0.0") {
					adresses[interface_name].name = interface_name;
					adresses[interface_name].ip_address = ip_address;
					adresses[interface_name].subnet_mask = network_interface.subnetMask().toString();
					adresses[interface_name].broadcast_address = network_interface.broadcastAddress().toString();
					printf("%s\n", adresses[interface_name].toString().c_str());
				}
			}
		}
	}
	return adresses;
}


#else

#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <net/if.h>
#include <net/if_dl.h>

map<string,string> getMyIPAddresses() {
    map<string,string> adresses;
    ifaddrs* ifap = NULL;
    int r = getifaddrs(&ifap);
    ifaddrs* current = ifap;
    while ( current ) {
        if ( current->ifa_addr && current->ifa_addr->sa_family == AF_INET ) {
            const sockaddr_in* interface_address = reinterpret_cast<const sockaddr_in*>(current->ifa_addr);
            string interface_name = current->ifa_name;
            string ip_address = inet_ntoa(interface_address->sin_addr);
            if ( ip_address != "127.0.0.1" && ip_address != "0.0.0.0" ) {
                adresses[ interface_name ] = ip_address;
            }
        }
        current = current->ifa_next;
    }
    return adresses;
}

#endif

static const int k_session_colour[4][3] = {
	{ 255, 0, 0 },
	{ 0, 255, 0 },
	{ 0, 0, 255 },
	{ 255, 255, 0 }
};

openTableClientSessionServer::openTableClientSessionServer() {
    
}
openTableClientSessionServer::~openTableClientSessionServer() {
    
}
//
//
//
void openTableClientSessionServer::setup(int port, int n_sessions) {
	//
	// create sessions and markers
	//
	cv::Mat marker;
	float hue = 0;
	float hue_inc = 255.0 / n_sessions;
	for (int i = 0; i < n_sessions; i++) {
		int marker_id = ARMarker::generateMarkerImage(marker);
		ofImage marker_image;
		marker_image.setFromPixels(marker.ptr(), marker.cols, marker.rows, OF_IMAGE_GRAYSCALE);
		marker_image.setAnchorPercent(0.5, 0.5);
		m_sessions[marker_id].setup(marker_id, marker_image );
		hue += hue_inc;
	}
	int j = 0;
	for (auto& session : m_sessions) {
		ofColor colour(k_session_colour[j][0], k_session_colour[j][1], k_session_colour[j][2]);
		session.second.setColour(colour);
		session.second.setStation(j);
		j++;
	}
	//
	// start server
	//
	m_tcp.setup(port);
	m_addresses = getMyIPAddresses();
	//
	//
	//
	m_last_broadcast = 0.;
	m_udp.Create();
	m_udp.SetEnableBroadcast(true);
	m_udp.SetNonBlocking(true);
	if (!m_udp.Connect(m_addresses.begin()->second.broadcast_address.c_str(), 8888)) {
		ofLogError("openTableClientSessionServer::setup : ") << "unable to connect udp";
	}
	//
	// setup leds
	//
	map< string, string > devices = openTableSerialDevice::listDevices();
	map< string, string >::iterator it = devices.begin();
	printf("Devices\n");
	for (; it != devices.end(); ++it) {
		printf("%s : %s\n", it->first.c_str(), it->second.c_str());
	}
	it = devices.find("led");
	if (it != devices.end()) {
		m_led.setup(it->second);
	}
}
void openTableClientSessionServer::close() {
    for ( auto& session : m_sessions ) {
        if ( !session.second.isAvailable() ) {
            m_tcp.disconnectClient(session.second.getClientId());
            session.second.release();
        }
    }
    m_tcp.close();
}
void openTableClientSessionServer::update() {
	ofApp* app = (ofApp*)ofGetAppPtr();
    //
    //
    //
    if ( ofGetElapsedTimef() - m_last_broadcast > 2. ) {
        if ( m_addresses.size() > 0 ) {
            char buffer[ 256 ];
            sprintf( buffer, "[%s]", m_addresses.begin()->second.ip_address.c_str());
            m_udp.Send(buffer, strlen(buffer));
        }
        m_last_broadcast = ofGetElapsedTimef();
    }
    //
    // process messages
    //
    for ( int i = 0; i < m_tcp.getLastID(); i++ ) {
        if ( m_tcp.isClientConnected(i) ) {
            string message = m_tcp.receive(i);
            if ( message.length() > 0 ) {
                //
                // process message
                //
                vector< string > parts = ofSplitString(message, "[~COMMAND~]");
                if ( parts.size() == 2 ) {
                    string command = parts[ 0 ];
                    vector<string> parameters = ofSplitString(parts[ 1 ], "[~PARAM~]");
                    ostringstream response;
                    response << command;
                    response << "[~COMMAND~]";
                    if ( command == "connect" ) {
                        response << parameters[ 0 ];
                        response << "[~PARAM~]";
                        bool connected = connectClient(i, parameters);
                        response << ( connected ? "connected" : "unavailable" );
						int session_id = ofToInt(parameters[0]);
                        if ( connected ) {
                            response << "[~PARAM~]";
                            
							ofColor session_colour = getSessionColour(session_id);
                            ostringstream colour_json;
                            colour_json << "{";
                            colour_json << "\"r\":";
                            colour_json << (int)(session_colour.r);
                            colour_json << ",";
                            colour_json << "\"g\":";
                            colour_json << (int)(session_colour.g);
                            colour_json << ",";
                            colour_json << "\"b\":";
                            colour_json << (int)(session_colour.b);
                            colour_json << "}";
                            response << colour_json.str();
							
                        }
						m_led.setStation(m_sessions[session_id].getStation(), connected);
                    } else if ( command == "disconnect" ) {
                        response << parameters[ 0 ];
                        response << "[~PARAM~]";
                        response << ( disconnectClient( i ) ? "disconnected" : "notconnected" );
                    } else if ( command == "document" ) {
                        ofxJSONElement json;
                        if ( json.parse(parameters[ 0 ] ) ) {
                            int session_id = getClientSession(i);
                            app->newDocument( session_id, json );
                        }
                    }
                    m_tcp.send(i, response.str());
                }
            }
        } else {
            disconnectClient( i );
        }
    }
}
void openTableClientSessionServer::draw() {
	//
	// display addresses
	//
	ofPoint p(10, 10);
	if (m_addresses.size() > 0) {
		ofDrawBitmapStringHighlight(m_addresses.begin()->second.ip_address, p);
	}
#ifdef _DEBUG
	/*
    for ( auto& adddress : m_addresses ) {
        string address_string = adddress.second.name + ":" + adddress.second.ip_address;
        ofDrawBitmapString(address_string, p);
        p.y += 20;
    }
	string hostname = getHostName();
	ofDrawBitmapString(hostname, p);
	*/
#endif    
    //
    // display markers
    //
    if ( m_sessions.size() > 0 ) {
        float scale = ofGetHeight() / 1080.;
        float marker_dim = 192. * scale;
        int max_columns = m_sessions.size() / 2;
        int column = 0;
        int row = 0;
        float column_width = ofGetWidth() / max_columns;
        p.x = column_width / 2.0;
        for ( auto& session : m_sessions ) {
            if ( row == 0 ) {
                p.y = ( marker_dim / 2.0 ) + ( 48*scale );
            } else {
                p.y = ofGetHeight() - ( ( marker_dim / 2.0 ) + ( 48*scale ) );
            }
            session.second.draw(p, marker_dim, marker_dim);
            if ( ++column >= max_columns ) {
                column = 0;
                row++;
                p.x = column_width / 2.0;
            } else {
                p.x += column_width;
            }
        }
    }
}
//
//
//
ofRectangle openTableClientSessionServer::getSafeZone() {
    float scale = ofGetHeight() / 1080.;
    float marker_dim = 192. * scale;
    float margin = 48. * scale;
    float exclusion = marker_dim + margin * 1.5;
    
    return ofRectangle( 0, exclusion, ofGetWidth(), ofGetHeight() - exclusion * 2 );
}

int openTableClientSessionServer::getSessionAtPoint( float x, float y ) {
    //
    //
    //
    if ( m_sessions.size() > 0 ) {
        ofPoint p( 10, 10 );
        float scale = ofGetHeight() / 1080.;
        float marker_dim = 192. * scale;
        int max_columns = m_sessions.size() / 2;
        int column = 0;
        int row = 0;
        float column_width = ofGetWidth() / max_columns;
        p.x = column_width / 2.0;
        for ( auto& session : m_sessions ) {
            if ( row == 0 ) {
                p.y = ( marker_dim / 2.0 ) + ( 48*scale );
            } else {
                p.y = ofGetHeight() - ( ( marker_dim / 2.0 ) + ( 48*scale ) );
            }
            ofRectangle r( p.x - ( marker_dim / 2.0 ), p.y - ( marker_dim / 2.0 ), marker_dim, marker_dim );
            if ( r.inside(x,y) ) {
                return session.first;
            }
           
            if ( ++column >= max_columns ) {
                column = 0;
                row++;
                p.x = column_width / 2.0;
            } else {
                p.x += column_width;
            }
        }
    }
    return -1;
}
bool openTableClientSessionServer::sendSessionMessage( int session_id, vector< string > message ) {
    ostringstream message_string;
    for ( int i = 0; i < message.size(); i++ ) {
        message_string << message[ i ];
        if ( i == 0 ) {
            message_string << "[~COMMAND~]";
        } else if ( i < message.size() - 1 ) {
            message_string << "[~PARAM~]";
        }
    }
    return sendSessionMessage( session_id, message_string.str() );
}
bool openTableClientSessionServer::sendSessionMessage( int session_id, string message )  {
    auto session = m_sessions.find(session_id);
    if ( session != m_sessions.end() ) {
        return m_tcp.send( session->second.getClientId(), message );
    }
    return false;
}
ofVec2f openTableClientSessionServer::getSessionPosition( int session_id ) {
    if ( m_sessions.size() > 0 ) {
        ofPoint p( 10, 10 );
        float scale = ofGetHeight() / 1080.;
        float marker_dim = 192. * scale;
        int max_columns = m_sessions.size() / 2;
        int column = 0;
        int row = 0;
        float column_width = ofGetWidth() / max_columns;
        p.x = column_width / 2.0;
        for ( auto& session : m_sessions ) {
            if ( row == 0 ) {
                p.y = ( marker_dim / 2.0 ) + ( 48*scale );
            } else {
                p.y = ofGetHeight() - ( ( marker_dim / 2.0 ) + ( 48*scale ) );
            }
            if ( session.first == session_id ) return p;
            if ( ++column >= max_columns ) {
                column = 0;
                row++;
                p.x = column_width / 2.0;
            } else {
                p.x += column_width;
            }
        }
    }
    return ofVec2f(ofGetWidth() /2.0, ofGetHeight() /2.0);
}
bool openTableClientSessionServer::isSessionConnected( int session_id ) {
    auto session = m_sessions.find(session_id);
    if ( session != m_sessions.end() ) {
        return !session->second.isAvailable();
    }
    return false;
}
//
//
//
int openTableClientSessionServer::testGetSessionAtPoint( float x, float y ) {
    //
    //
    //
    if ( m_sessions.size() > 0 ) {
        ofPoint p( 10, 10 );
        float scale = ofGetHeight() / 1080.;
        float marker_dim = 192. * scale;
        int max_columns = m_sessions.size() / 2;
        int column = 0;
        int row = 0;
        float column_width = ofGetWidth() / max_columns;
        p.x = column_width / 2.0;
        for ( auto& session : m_sessions ) {
            if ( row == 0 ) {
                p.y = ( marker_dim / 2.0 ) + ( 48*scale );
            } else {
                p.y = ofGetHeight() - ( ( marker_dim / 2.0 ) + ( 48*scale ) );
            }
            ofRectangle r( p.x - ( marker_dim / 2.0 ), p.y - ( marker_dim / 2.0 ), marker_dim, marker_dim );
            if ( r.inside(x,y) ) {
                return session.first;
            }
            
            if ( ++column >= max_columns ) {
                column = 0;
                row++;
                p.x = column_width / 2.0;
            } else {
                p.x += column_width;
            }
        }
    }
    return -1;
    
}
bool openTableClientSessionServer::testToggleSession( int session_id ) {
    auto session = m_sessions.find(session_id);
    if ( session != m_sessions.end() ) {
        if ( session->second.isAvailable()) {
            session->second.aquire(100);
        } else {
            session->second.release();
        }
        return true;
    }
    return false;
}
//
//
//
bool openTableClientSessionServer::connectClient( int client_id, vector< string >& parameters ) {
    int session_id = ofToInt(parameters[ 0 ]);
    auto session = m_sessions.find(session_id);
    if ( session != m_sessions.end() && session->second.isAvailable() ) {
        session->second.aquire(client_id);
        return true;
    }
    return false;
}
bool openTableClientSessionServer::disconnectClient( int client_id ) {
    for ( auto& session : m_sessions ) {
        if ( session.second.getClientId() == client_id ) {
            session.second.release();
			m_led.setStation( session.second.getStation(), false);
            return true;
        }
    }
    return false;
}

int openTableClientSessionServer::getClientSession( int client_id ) {
    for ( auto& session : m_sessions ) {
        if ( session.second.getClientId() == client_id ) {
            return session.second.getSessionId();
        }
    }
    return -1;
}

ofColor openTableClientSessionServer::getSessionColour( int session_id ) {
	auto session = m_sessions.find(session_id);
	if (session != m_sessions.end()) {
		return session->second.getColour();
	}
	return ofColor::black;
}

