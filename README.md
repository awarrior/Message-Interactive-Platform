# Message Interactive Platform (MIP) - 2011

A platform designed for the purpose of interaction between audiences and host utilizing phone short message.

This platform needs one nokia phone (also the cable driver) as a basic message receiver, one laptop/pc for processing as a message center and one projector for showing as a secondary screen. Once it is established, the audiences can send message to one specified phone number using their own phones.

The primary technique used here is C++ MFC framework, nokia SDK project and Flash ActionScript lauguage. The communications between receiver and processor, or between processor and projector, use socket way and thread barrier is used for synchronizing the running threads.

Three features are available in this project: comment, voting and lottery. For comment function, Sunday string searching algorithm is used for matching some specified key words (e.g. message prefix, sensitive word). Also, we could set the refreshing time of projector or the receiving time of processor. For voting function, we could set the maximum votes, voting items and refreshing time. For the last function, lottery provides settings: participants and opportunity controller.

WARNING: this platform is not suitable to fast and mass message interaction scene, for the communication limit of one phone.
