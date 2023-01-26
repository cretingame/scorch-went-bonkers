#include <cmath>
#include <GL/gl.h>
#include <alleggl.h>

#include "../engine/GLTexture.h"
#include "../misc/Random.h"
#include "Terrain.h"

/*********************************************************/

GLTexture *Terrain::soilTxtr = NULL;
GLTexture *Terrain::grassTxtr = NULL;

/*********************************************************/

void Terrain::loadBitmaps()
{
	soilTxtr =  GLTexture::load("gfx/terrain/soil_texture.tga", GL_RGB8, 0);
	grassTxtr = GLTexture::load("gfx/terrain/grass_texture.tga", GL_RGBA8, AGL_TEXTURE_MASKED);
	
	grassTxtr->bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void Terrain::freeBitmaps()
{
	delete soilTxtr;
	delete grassTxtr;
}

/*********************************************************/

Terrain::Terrain()
	: Entity(0.0, 600.0)
{
	
	height[0] = randomFloat(100.0, 300.0);
	height[1] = height[0] + randomFloat(-5.0, 5.0);
	
	for (int i = 2; i < TERRAIN_SURFACE_POINTS; i++)
	{
		double prevDiff = height[i-1] - height[i-2];
		double prevHeight = height[i-1];
		
		double newDiff = prevDiff + randomFloat(-2.0 - ((prevHeight - 200.0) / 250.0), 2.0 - ((prevHeight - 250.0) / 150.0));
		if (std::abs(newDiff) > 9.0)
			newDiff = newDiff / std::abs(newDiff) * 7.0; 
		
		height[i] = prevHeight + newDiff;
	}
}

/*********************************************************/

double Terrain::getHeightAt(double x)
{
	double pointWidth = 800.0 / (TERRAIN_SURFACE_POINTS - 1);
	int left = (int)(x / pointWidth);
	double offset = (x - left * pointWidth) / pointWidth;
	
	return height[left] + offset * (height[left + 1] - height[left]);
}

/*********************************************************/

double Terrain::getYAt(double x)
{
	return 594.0 - getHeightAt(x);
}

/*********************************************************/

double Terrain::getNormalAngleAt(double x)
{
	double left  = (x > 5.0) ? x - 5.0 : 0.0;
	double right = (x < 795.0) ? x + 5.0 : 0.0;
	double hL = getHeightAt(left), hR = getHeightAt(right);
	
	double diffX = hL - hR, diffY = right - left;
	return std::atan2(diffY, diffX);
}

/*********************************************************/

bool Terrain::contains(double x, double y)
{
	if ((x < 0.0) || (x > 800.0)) return false;
	if (y <= 598.0 - getHeightAt(x)) return false;

	return true;
}
/*********************************************************/

void Terrain::flattenAt(double x)
{
	double pointWidth = 800.0 / (TERRAIN_SURFACE_POINTS - 1);
	int left = (int)(x / pointWidth);
	
	double average = 0.0;
	for (int i = left - 8; i <= left + 8; i++)
		average += height[i];
	average /= 17;
	
	for (int i = left - 3; i <= left + 3; i++)
		if ((i > 0) && (i < TERRAIN_SURFACE_POINTS))
			height[i] = average;
	
	for (int i = 1; i < 5; i++)
	{
		if ((left - 8 + i > 0) && (left - 8 + i < TERRAIN_SURFACE_POINTS))
			height[left - 8 + i] = (height[left-8] * (5-i) + height[left-3] * i) / 5;
		if ((left + 3 + i > 0) && (left + 3 + i < TERRAIN_SURFACE_POINTS))
			height[left + 3 + i] = (height[left+3] * (5-i) + height[left+8] * i) / 5;
	}
}

/*********************************************************/

void Terrain::render()
{
	glPushMatrix();
	glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT);
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	soilTxtr->bind();
	
	glTranslatef(x, y, 0.0f);
	
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < TERRAIN_SURFACE_POINTS; i++)
	{
		double x = (double)i * 800 / (TERRAIN_SURFACE_POINTS - 1);
		glTexCoord2f(x / 128.0, 0.0);
		glColor3f(0.5, 0.5, 0.5);
		glVertex2f(x, 0.0);
		glTexCoord2f(x / 128.0, height[i] / 128.0);
		
		double diff = (i > 0) ? (height[i] - height[i - 1]) : (height[1] - height[0]);
		double colorValue = 0.7 + height[i] * 0.5 / 400.0 + diff * 0.2 / 7.0;
		glColor3f(colorValue, colorValue, colorValue);
		glVertex2f(x, -height[i]);
	}
	glEnd();
	
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.5);
	grassTxtr->bind();
	
	double pointWidth = 800.0 / (TERRAIN_SURFACE_POINTS - 1);
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < TERRAIN_SURFACE_POINTS; i++)
	{
		double x = (double)i * 800 / (TERRAIN_SURFACE_POINTS - 1);
		double diff = (i > 0) ? (height[i] - height[i - 1]) : (height[1] - height[0]);
		double hFactor = pointWidth / std::sqrt(pointWidth * pointWidth + diff * diff) + 0.02 * std::abs(diff);//1.0 - std::abs(diff) * 0.08;

		double colorValue = 0.5 + height[i] * 0.5 / 400.0 + diff * 0.2 / 7.0;
		glColor3f(colorValue, colorValue, colorValue);
		
		glTexCoord2f(x / 32.0, 0.0);
		glVertex2f(x, -height[i] + 3.0 / hFactor);
		glTexCoord2f(x / 32.0, 1.0);
		glVertex2f(x, -height[i] - 7.0 / hFactor);
	}
	glEnd();
	
	glPopAttrib();
	glPopMatrix();
}
