//
// Copyright (C) 2014 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//


#include "inet/common/geometry/container/BvhTree.h"

#include <algorithm>
#include <limits>

#include "inet/common/geometry/shape/Cuboid.h"

namespace inet {

using namespace physicalenvironment;

bool BvhTree::isLeaf() const
{
    return objects.size() != 0;
}

BvhTree::BvhTree(const Coord& boundingMin, const Coord& boundingMax, std::vector<const IPhysicalObject *>& objects, unsigned int start, unsigned int end, Axis axis, unsigned int leafCapacity)
{
    this->left = nullptr;
    this->right = nullptr;
    this->boundingMin = boundingMin;
    this->boundingMax = boundingMax;
    this->center = (boundingMax - boundingMin) / 2 + boundingMin;
    this->leafCapacity = leafCapacity;
    buildHierarchy(objects, start, end, axis);
}

void BvhTree::buildHierarchy(std::vector<const IPhysicalObject *>& objects, unsigned int start, unsigned int end, Axis axis)
{
    if (end - start + 1 <= leafCapacity) {
        for (unsigned int i = start; i <= end; i++)
            this->objects.push_back(objects[i]);
    }
    else {
        auto s = objects.begin();
        auto e = s;
        std::advance(s, start);
        std::advance(e, end + 1);
        sort(s, e, AxisComparator(axis.getCurrentAxis()));
        axis.getNextAxis();
        Coord boundingMin, boundingMax;
        computeBoundingBox(boundingMin, boundingMax, objects, start, (start + end) / 2);
        left = new BvhTree(boundingMin, boundingMax, objects, start, (start + end) / 2, axis, leafCapacity);
        computeBoundingBox(boundingMin, boundingMax, objects, 1 + (start + end) / 2, end);
        right = new BvhTree(boundingMin, boundingMax, objects, 1 + (start + end) / 2, end, axis, leafCapacity);
    }
}

void BvhTree::computeBoundingBox(Coord& boundingMin, Coord& boundingMax, std::vector<const IPhysicalObject *>& objects, unsigned int start, unsigned int end) const
{
    double xMin = std::numeric_limits<double>::max();
    double yMin = xMin;
    double zMin = xMin;
    double xMax = -std::numeric_limits<double>::max();
    double yMax = xMax;
    double zMax = xMax;
    for (unsigned int i = start; i <= end; i++) {
        const IPhysicalObject *phyObj = objects[i];
        Coord pos = phyObj->getPosition();
        Coord size = phyObj->getShape()->computeBoundingBoxSize();
        size /= 2;
        Coord objMaxBounding = pos + size;
        Coord objMinBounding = pos - size;
        if (objMaxBounding.x > xMax)
            xMax = objMaxBounding.x;
        if (objMaxBounding.y > yMax)
            yMax = objMaxBounding.y;
        if (objMaxBounding.z > zMax)
            zMax = objMaxBounding.z;
        if (objMinBounding.x < xMin)
            xMin = objMinBounding.x;
        if (objMinBounding.y < yMin)
            yMin = objMinBounding.y;
        if (objMinBounding.z < zMin)
            zMin = objMinBounding.z;
    }
    boundingMin = Coord(xMin, yMin, zMin);
    boundingMax = Coord(xMax, yMax, zMax);
}

bool BvhTree::intersectWithLineSegment(const LineSegment& lineSegment) const
{
    Coord size = Coord(boundingMax.x - boundingMin.x, boundingMax.y - boundingMin.y, boundingMax.z - boundingMin.z);
    Coord p0 = lineSegment.getPoint1() - center;
    Coord p1 = lineSegment.getPoint2() - center;
    Cuboid cuboid(size);
    LineSegment translatedLineSegment(p0, p1);
    Coord intersection1, intersection2, normal1, normal2; // TODO implement a bool computeIntersection(lineSegment) function
    return cuboid.computeIntersection(translatedLineSegment, intersection1, intersection2, normal1, normal2);
}

void BvhTree::lineSegmentQuery(const LineSegment& lineSegment, const IVisitor *visitor) const
{
    if (isLeaf()) {
        for (auto& elem : objects)
            // TODO avoid dynamic_cast
            visitor->visit(dynamic_cast<const cObject *>(elem));
    }
    else if (intersectWithLineSegment(lineSegment)) {
        left->lineSegmentQuery(lineSegment, visitor);
        right->lineSegmentQuery(lineSegment, visitor);
    }
}

BvhTree::~BvhTree()
{
    delete left;
    delete right;
}

} /* namespace inet */

