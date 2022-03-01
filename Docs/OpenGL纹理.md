# OpenGL纹理

+ 三角形纹理坐标

```cpp
GLfloat texCoords[] = {
		0.0f, 0.0f, // 左下角
		1.0f, 0.0f, // 右下角
		0.5f, 1.0f // 上方中点
	};
```



+ 纹理环绕方式（wrapping）:

    + `GL_REPEAT` :默认行为，重复纹理
    + `GL_MIRRORED_REPEAT` : 重复纹理但是是镜像放置
    + `GL_CLAMP_TO_EDGE` : 约束到[0,1],超出部分重复纹理坐标边缘，产生边缘被拉伸效果
    + `GL_CLAMP_TO_BORDER`  : 超出部分为用户指定边缘色

+ 设置纹理环绕方式:

  ```cpp
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // s轴，还有t,r轴，对应x,y,z
  float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
  ```

+ 纹理过滤

  + `GL_NEAREST `:  最邻近过滤，8bit风格

  + `GL_LINEAR`: 线性过滤，更平滑
+ 放大缩小时可设置纹理过滤
```cpp
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
```
+ 多级渐远纹理(Mipmap):**注意只有缩小时才会使用多级纹理过滤** 
  + 创建纹理后调用glGenerateMipmaps函数OpenGL之后会**自动创建多级渐远纹理**
  + 与Mipmap组合的纹理过滤方式：
    + `GL_NEAREST_MIPMAP_NEAREST`
    + `GL_LINEAR_MIPMAP_NEAREST`
    + `GL_NEAREST_MIPMAP_LINEAR`
    + `GL_LINEAR_MIPMAP_LINEAR`

+ 生成纹理流程：

```cpp

// 生成纹理
GLuint texture;
glGenTextures(1, &texture); // 第一个表示生成纹理数量，第二个为纹理存储单元，多个则使用GLuint数组存储
glBindTexture(GL_TEXTURE_2D, texture);

// 为当前绑定的纹理对象设置环绕、过滤方式
...

// 加载纹理图片
int width, height;
const char* filePath = "./Assert/container.jpg";
unsigned char* image = SOIL_load_image(filePath, &width, &height, 0, SOIL_LOAD_RGB); // 会返回宽高到对应变量中

// 纹理对象附加纹理图片
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
/*glTexImage2D
* params[1]: 指定生成纹理的目标，因为之前绑定的时2D纹理
* params[2]: 纹理存为何种格式（我们使用的图像只有rgb,so ...）
* params[3]: 指定多级渐远纹理级别， 0为基本级别
* params[4-5]:指定纹理最终宽高， 这里使用了加载纹理图片时的宽高
* params[6]: 历史遗留
* params[7-8]: 源图格式和数据类型
* last params: 真正图像数据
*/
glGenerateMipmap(GL_TEXTURE_2D); // 自动生成多级渐远纹理，从而不需要我们不断向glTexImage2D传入不同级别

// 生成纹理和对应多级纹理后： 释放内存和解绑纹理对象
SOIL_free_image_data(image);
glBindTexture(GL_TEXTURE_2D, 0);
```