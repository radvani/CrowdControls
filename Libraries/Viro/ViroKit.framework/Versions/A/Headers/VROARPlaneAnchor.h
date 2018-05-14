//
//  VROARPlaneAnchor.h
//  ViroRenderer
//
//  Created by Raj Advani on 6/11/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROARPlaneAnchor_h
#define VROARPlaneAnchor_h

#include "VROARAnchor.h"
#include "VROVector3f.h"

enum class VROARPlaneAlignment {
    Horizontal = 0x1,
    HorizontalUpward = 0x11,
    HorizontalDownward = 0x101,
    Vertical = 0x10,
};

/*
 Anchor representing a planar surface.
 */
class VROARPlaneAnchor : public VROARAnchor {
    
public:
    
    VROARPlaneAnchor() {}
    virtual ~VROARPlaneAnchor() {}
    
    /*
     The approximate alignment of the detected plane.
     */
    VROARPlaneAlignment getAlignment() const {
        return _alignment;
    }
    void setAlignment(VROARPlaneAlignment alignment) {
        _alignment = alignment;
    }
    
    /*
     The center point of the detected plane. Relative to the parent
     anchor position.
     */
    VROVector3f getCenter() const {
        return _center;
    }
    void setCenter(VROVector3f center) {
        _center = center;
    }
    
    /*
     The width and length of the detected plane.
     */
    VROVector3f getExtent() const {
        return _extent;
    }
    void setExtent(VROVector3f extent) {
        _extent = extent;
    }

    void setBoundaryVertices(std::vector<VROVector3f> points) {
        _boundaryVertices = points;
    }

    std::vector<VROVector3f> getBoundaryVertices() {
        return  _boundaryVertices;
    }
    
private:
    
    /*
     The approximate alignment of the detected plane.
     */
    VROARPlaneAlignment _alignment;
    
    /*
     The center point of the detected plane. Relative to the parent
     anchor position.
     */
    VROVector3f _center;

    /*
     The width and length of the detected plane.
     */
    VROVector3f _extent;

    /*
     A vector of points representing the vertex boundaries of this plane, if any.
     */
    std::vector<VROVector3f> _boundaryVertices;
};

#endif /* VROARPlaneAnchor_h */
