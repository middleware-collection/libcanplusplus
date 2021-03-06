/*!
 * @file 	SDOMsg.hpp
 * @brief	Type definitions
 * @author 	Christian Gehring
 * @date 	Jan, 2012
 * @version 1.0
 * @ingroup robotCAN
 *
 */
#ifndef SDOMSG_HPP_
#define SDOMSG_HPP_

#include "CANOpenMsg.hpp"
#include <boost/shared_ptr.hpp>


//! Service Data Object Message Container
/*! It contains an output CANOpen message that needs to be sent to the CAN node and
 *  an input CANOpen message that stores the answer received from the CAN node.
 *  It also stores information about the status of the SDO that is
 *  	queuing:	not yet processed by the SDO manager
 *  	sent:		output message was sent to CAN node
 *  	waiting:	is waiting for a response of the CAN node
 *  	received:	has received a response from the CAN node
 *  	timeout:	has not received a response after a while
 *
 * @ingroup robotCAN
 */
class SDOMsg {
public:
	/*! Constructor
	 *
	 * @param inSMID	index of shared memory of input message
	 * @param outSMID   index of shared memory of output message
	 * @param nodeId	ID of the CAN node
	 */
	SDOMsg(int inSMID, int outSMID, int nodeId);

	//! Destructor
	virtual ~SDOMsg();

	/*! Gets the reference to the output message
	 * @return	 output message
	 */
	CANOpenMsg* getOutputMsg();

	/*! Gets the reference to the input message
	 * @return	 input message
	 */
	CANOpenMsg* getInputMsg();

	/*! Gets flag
	 * @return true if input message was not received in a certain time
	 */
	bool hasTimeOut();

	/*! Gets flag
	 * @return true if output message was sent to the node
	 */
	bool getIsSent();

	/*! Gets flag
	 * @return	true if input message was received from node
	 */
	bool getIsReceived();

	/*! Gets flag
	 * @return true if SDO is pending for receiving an answer from the node
	 */
	bool getIsWaiting();

	/*! Gets flag
	 * @return true if SDO is queuing to be processed by the SDO manager, i.e. the
	 * 	message neither was sent nor is waiting for an input message
	 */
	bool getIsQueuing();

	/*! The SDO manager sets the isWaiting flag to true as soon as it has sent
	 * the output message
	 * @param isWaiting
	 */
	virtual void setIsWaiting(bool isWaiting);

	/*! Sets the flag that the SDO was added to the SDOManager
	 * @param isQueuing
	 */
	virtual void setIsQueuing(bool isQueuing);

	/*! Gets the output CAN message that needs to be sent to the CAN node
	 * @param[out] canDataDes	output CAN message
	 */
	void sendMsg(CAN_BusDataDes *canDataDes);

	/*! Sets the input CAN message that was received from the CAN node
	 * @param[in] canDataMeas	input CAN message
	 */
	void receiveMsg(CAN_BusDataMeas *canDataMeas);



protected:
	//! Hook function that is invoked when a message is received
	virtual void processReceivedMsg();

	//! CAN node ID
	int nodeId_;

	//! timeout counter
	int timeout_;

	//! if true, SDO manager has sent SDO through CAN network
	bool isSent_;

	//! if true, response from node is received
	bool isReceived_;

	//! if true, SDO was sent and SDO manager is waiting for a response of the node
	bool isWaiting_;

	//! if true, SDO was added to the SDO manager, but not yet sent
	bool isQueuing_;

	//! input CAN message that is received from the CAN node
	CANOpenMsg* inputMsg_;

	//! output CAN message that will be sent to the CAN node
	CANOpenMsg* outputMsg_;
};

//! Boost shared point of an SDO message
typedef boost::shared_ptr<SDOMsg> SDOMsgPtr;

#endif /* SDOMSG_HPP_ */
