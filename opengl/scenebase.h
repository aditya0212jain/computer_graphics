#include <list>
#include <glm/glm.hpp>

class GeomBase;
class Camera;
class Renderer;

struct SceneBase {
private:
   const GeomBase *object;
   glm::mat4   objmat;

   SceneBase *sibling;
   SceneBase *firstchild;
public:
   
   inline void set(const GeomBase *g) { object = g; }
   inline void set(glm::mat4 m) { objmat = m; }
   inline void set(SceneBase *ch) { firstchild = ch; }
   SceneBase() {} // For a child to manage
   SceneBase(const GeomBase *obj, glm::mat4 mat = glm::mat4(1.0f),
             SceneBase *next=NULL, SceneBase *child=NULL);
   void init(const GeomBase *obj, glm::mat4 mat, SceneBase *next, SceneBase *child);
   int render(Renderer *renderer, const Camera &camera, const glm::mat4 mat = glm::mat4(1.0f));
   int render(Renderer *renderer, const Camera &camera, const float *mat=NULL);
   SceneBase* addchild(const GeomBase *obj, glm::mat4 mat = glm::mat4(1.0f));
   void attachSibling(SceneBase* sib);
   void attachChild(SceneBase* ch);
   void transformScene(glm::mat4 rotate_mat);
   void updateScene(int vao_id,glm::mat4 transform);
};
