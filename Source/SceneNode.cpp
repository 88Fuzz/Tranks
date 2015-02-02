#include "SceneNode.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>

//TODO figure this out
std::queue<SceneNode *> g_drawingQ;

//TODO update with comments here
SceneNode::SceneNode(Category::Type category) :
        children(), parent(NULL), type(category)
{
}

SceneNode::~SceneNode()
{
    for(std::vector<SceneNode *>::iterator it = children.begin(); it != children.end(); it++)
    {
        delete *(it);
    }
}

void SceneNode::attachChild(SceneNode *child)
{
    child->parent = this;
    children.push_back(child);
}

SceneNode* SceneNode::detachChild(SceneNode* node)
{
    SceneNode *result = NULL;
    for(std::vector<SceneNode *>::iterator it = children.begin(); it != children.end(); it++)
    {
        //TODO look into to see if there is a better way to remove a child
        if(node == (*it))
        {
            children.erase(it);
            result = *it;
            break;
        }
    }
    //TODO do some kind of logging yo
    if(result != NULL)
    {
        result->parent = NULL;
    }
    else
    {
        std::cerr << "Could not find child to remove\n";
    }

    return result;
}

void SceneNode::update(sf::Time dt)
{
    updateCurrent(dt);
    updateChildren(dt);
}

void SceneNode::updateCurrent(sf::Time dt)
{
    // Do nothing by default
}

void SceneNode::updateChildren(sf::Time dt)
{
    for(std::vector<SceneNode *>::iterator it = children.begin(); it != children.end(); it++)
    {
        (*it)->update(dt);
    }
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // Apply transform of current node
    states.transform *= getTransform();

    /*
     * Draw node and children with changed transform, in a depth first fashion
     */
//    drawCurrent(target, states);
//    drawChildren(target, states);
    /*
     * Draw node and childen with changed transform, in a breadth first fashion
     */
    //Top of Queue will be current, pop it off
    //Check if empty for root node, which wont be pushed into queue
    if(!g_drawingQ.empty())
        g_drawingQ.pop();

    drawCurrent(target, states);

    for(std::vector<SceneNode *>::const_iterator it = children.begin(); it != children.end(); it++)
    {
        if((*it)->isAlive())
            g_drawingQ.push(*it);
    }

    if(!g_drawingQ.empty())
        g_drawingQ.front()->draw(target, states);

// Draw bounding rectangle - disabled by default
//drawBoundingRect(target, states);
}

/*
 * If sceneNode is alive, it will be added to the drawQ.
 * Return true if alive, false if not alive
 */
bool SceneNode::isAlive()
{
    return true;
}
void SceneNode::drawCurrent(sf::RenderTarget&, sf::RenderStates) const
{
// Do nothing by default
}

void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
    for(std::vector<SceneNode *>::const_iterator it = children.begin(); it != children.end(); it++)
    {
        (*it)->draw(target, states);
    }
}

/*
 * Used to show hit boxes
 */
void SceneNode::drawBoundingRect(sf::RenderTarget& target, sf::RenderStates) const
{
    sf::FloatRect rect = getBoundingRect();

    sf::RectangleShape shape;
    shape.setPosition(sf::Vector2f(rect.left, rect.top));
    shape.setSize(sf::Vector2f(rect.width, rect.height));
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(sf::Color::Green);
    shape.setOutlineThickness(1.f);

    target.draw(shape);
}

sf::Vector2f SceneNode::getWorldPosition() const
{
    return getWorldTransform() * sf::Vector2f();
}

sf::Transform SceneNode::getWorldTransform() const
{
    sf::Transform transform = sf::Transform::Identity;

    for(const SceneNode* node = this; node != nullptr; node = node->parent)
        transform = node->getTransform() * transform;

    return transform;
}

/*
 * Wait until I need this to implement it
 */
void SceneNode::onCommand(const Command* command, sf::Time dt)
{
// Command current node, if category matches
//if(command->category & getCategory())
//    command->action(*this, dt);
//
//// Command children
//FOREACH(Ptr & child, mChildren)
//child->onCommand(command, dt);
}

unsigned int SceneNode::getCategory() const
{
    return type;
}

sf::FloatRect SceneNode::getBoundingRect() const
{
    return sf::FloatRect();
}

bool SceneNode::isDestroyed() const
{
// By default, scene node needn't be removed
    return false;
}

SceneNode* SceneNode::getChildNode(int pos)
{
    if(children.size() < pos)
        return NULL;
    return children[pos];
}

/*
 * returns player node at position pos
 */
Player* SceneNode::removePlayerChildNode(int pos)
{
    if(children.size() < pos)
        return NULL;

    return children[pos]->findAndRemovePlayer();
}

/*
 * returns any player at node. Will remove player from node. If none is present, return null
 */
Player* SceneNode::findAndRemovePlayer()
{
    SceneNode *player = NULL;
    for(int j = 0; j < children.size(); j++)
    {
        if(children[j]->getCategory() == Category::PLAYER)
        {
            player = children[j];
            children.erase(children.begin() + j);
            return (Player *) player;
        }
    }
    return NULL;
}

/*
 * Finds any player at node, return reference if found. Else return null
 */
SceneNode* SceneNode::findPlayer()
{
    for(int j = 0; j < children.size(); j++)
    {
        if(children[j]->getCategory() == Category::PLAYER)
        {
            return children[j];
        }
    }
    return NULL;
}

/*
 * returns BoardPiece if child pos is of type type, else NULL
 */
SceneNode* SceneNode::getChildNode(int pos, Category::Type type)
{
    if(children.size() <= pos)
        return NULL;

    if(children[pos]->getCategory() & type)
        return children[pos];

    return children[pos]->getChildNode(pos, type);
}

SceneNode* SceneNode::getChildNode(int pos, Category::Type type, int layer)
{
    if(children.size() < pos)
        return NULL;

    return children[pos]->findPlayer();
}
