OBJS = src/engine/FPSCounter.o  \
  src/engine/TimeKeeper.o  \
  src/engine/TimeRoutines.o \
  src/engine/GLBitmap.o \
  src/engine/GLTexture.o \
  src/engine/TextRenderer.o \
  src/decor/Trail.o \
  src/decor/SmokeTrail.o \
  src/decor/TextFloat.o \
  src/decor/Cloud.o \
  src/entity/GameWorld.o \
  src/entity/Sorters.o \
  src/entity/Airborne.o \
  src/game/Game.o \
  src/game/TargetGame.o \
  src/game/BattleGame.o \
  src/game/FragGame.o \
  src/objects/Terrain.o \
  src/objects/Tank.o \
  src/objects/Bullet.o \
  src/objects/Explosion.o \
  src/objects/Shield.o \
  src/objects/GroundDebris.o \
  src/objects/GroundFountain.o \
  src/objects/Background.o \
  src/objects/Scoreboard.o \
  src/objects/Target.o \
  src/objects/TimeCounter.o \
  src/objects/TargetBoard.o \
  src/control/SwitchControl.o \
  src/control/DroneControl.o \
  src/control/FakeBullet.o \
  src/menu/Menu.o \
  src/menu/KeySelectMenu.o \
  src/menu/MenuOption.o \
  src/menu/AscenderOption.o \
  src/menu/MultiOption.o \
  src/menu/StaticText.o \
  src/menu/StaticImage.o \
  src/menu/SplashScreen.o \
  src/misc/Random.o \
  src/misc/TextHandler.o \
  src/misc/KeyReader.o \
  src/misc/Settings.o \
  src/misc/ScreenFader.o \
  src/misc/Quitting.o \
  src/misc/Theme.o \
  src/player/Player.o \
  src/sound/SoundSystem.o \
  src/MainMenu.o \
  src/Main.o

SRCS = $(subst .o,.cpp,$(OBJS))
OBJS_D = $(subst /,\,$(OBJS))
DEPENDS = Makefile.dep
LIBS = -static-libgcc -static-libstdc++ -lfmod -ljpgalleg -lalleggl -lalleg44 -lglu32 -lopengl32 -luser32 -lgdi32 -lwinmm
EXENAME = swb.exe

CXX = g++
RM = del /q
MV = ren

CXXFLAGS = -Wall -g
DEPFLAGS = -MM

DISTEXENAME = scorchwentbonkers.exe
DISTFLAGS = -Wall -s -O3
DISTNAME = scorch-went-bonkers-win.zip
DISTFILES = fonts gfx misc mus snd alleg44.dll fmod.dll LICENSE

SRCDISTNAME = scorch-went-bonkers-src.zip
SRCDISTFILES = Makefile Makefile.dep LICENSE src fonts gfx misc mus snd

$(EXENAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) src/icon.res $(LIBS) -o $(EXENAME)

$(DISTEXENAME): $(OBJS)
	$(CXX) $(DISTFLAGS) $(OBJS) src/icon.res $(LIBS) -o $(DISTEXENAME)

unlink:
	$(RM) $(EXENAME)
	$(RM) $(DISTEXENAME)
	$(RM) $(DISTNAME)
	$(RM) $(SRCDISTNAME)

clean:
	$(RM) $(OBJS_D)
	$(RM) $(EXENAME)
	$(RM) $(DISTEXENAME)
	$(RM) $(DISTNAME)
	$(RM) $(SRCDISTNAME)

veryclean: clean
	$(RM) $(DEPENDS)
	
Makefile.dep:
	@echo Generating dependencies...
	@$(CXX) $(DEPFLAGS) $(SRCS) >$(DEPENDS)
	@sed "s/.*\.o: \(.*\)\.cpp/\1.o: \1.cpp/" <$(DEPENDS) >$(DEPENDS)2
	@$(RM) $(DEPENDS)
	@$(MV) $(DEPENDS)2 $(DEPENDS)
	@echo Done.

depend:
	@echo Generating dependencies...
	@$(CXX) $(DEPFLAGS) $(SRCS) >$(DEPENDS)
	@sed "s/.*\.o: \(.*\)\.cpp/\1.o: \1.cpp/" <$(DEPENDS) >$(DEPENDS)2
	@$(RM) $(DEPENDS)
	@$(MV) $(DEPENDS)2 $(DEPENDS)
	@echo Done.

$(DISTNAME): $(DISTEXENAME)
	zip -ur9 $(DISTNAME) $(DISTEXENAME) $(DISTFILES)

$(SRCDISTNAME):
	zip -ur9 $(SRCDISTNAME) $(SRCDISTFILES) -x *.o

dist: $(DISTNAME) $(SRCDISTNAME)
	@echo Distribution packages built.

include $(DEPENDS)

.PHONY: clean veryclean depend dist unlink
