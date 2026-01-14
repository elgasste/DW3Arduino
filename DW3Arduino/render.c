#include "game.h"
#include "utility.h"

internal void Render_DrawTileMapLayer( Game_t* game, void ( *layerFunc )( Game_t*, i32, i32, i32, i32, i32, i32 ) );
internal void Render_DrawTileMapSection( Game_t* game, i32 vx, i32 vy, i32 vw, i32 vh, i32 xOffset, i32 yOffset );
internal void Render_DrawStaticSpritesInSection( Game_t* game, i32 vx, i32 vy, i32 vw, i32 vh, i32 xOffset, i32 yOffset );
internal void Render_DrawEntitiesInSection( Game_t* game, i32 vx, i32 vy, i32 vw, i32 vh, i32 xOffset, i32 yOffset );
internal void Render_SortEntities( Entity_t* entities, u32 entityCount, Entity_t** sortedEntities, u32* sortCount, Bool_t reverseOrder );
internal void Render_SortEntity( Entity_t* entity, Entity_t** sortedEntities, u32* sortCount );
internal void Render_DrawSortedEntities( Game_t* game, Entity_t** sortedEntities, u32 entityCount, i32 vx, i32 vy, i32 vw, i32 vh, i32 xOffset, i32 yOffset );

void Render_DrawGame( Game_t* game )
{
   Screen_WipeColor( &game->screen, SCREEN_COLOR16_BLACK );

   if ( game->state < GameState_Intro_Count )
   {
      // TODO: render intro stuff
   }
   else if ( game->state < GameState_Overworld_Count )
   {
      Render_DrawTileMapLayer( game, Render_DrawTileMapSection );
      Render_DrawTileMapLayer( game, Render_DrawStaticSpritesInSection );
      Render_DrawTileMapLayer( game, Render_DrawEntitiesInSection );
   }

   Screen_Blit( &game->screen );
}

internal void Render_DrawTileMapLayer( Game_t* game, void ( *layerFunc )( Game_t*, i32, i32, i32, i32, i32, i32 ) )
{
   i32 mapW, mapH;
   i32 x, y, w, h, ivx, ivy, ivw, ivh;

   ivx = game->tileMap.viewport.x < 0.0f ? (i32)( game->tileMap.viewport.x - 1.0f ) : (i32)game->tileMap.viewport.x;
   ivy = game->tileMap.viewport.y < 0.0f ? (i32)( game->tileMap.viewport.y - 1.0f ) : (i32)game->tileMap.viewport.y;
   ivw = (i32)game->tileMap.viewport.w;
   ivh = (i32)game->tileMap.viewport.h;

   layerFunc( game, ivx, ivy, ivw, ivh, 0, 0 );

   if ( game->tileMap.wraps )
   {
      mapW = game->tileMap.tilesX * TILEMAP_TILE_SIZE;
      mapH = game->tileMap.tilesY * TILEMAP_TILE_SIZE;

      if ( ivx < 0 ) // draw "left" map
      {
         x = mapW + ivx;
         y = ivy;
         h = ivh;
         w = -ivx;
         layerFunc( game, x, y, w, h, 0, 0 );

         if ( ivy ) // draw "top left" map
         {
            y = mapH + ivy;
            h = -ivy;
            layerFunc( game, x, y, w, h, 0, 0 );
         }

         if ( ( ivy + ivh ) > mapH ) // draw "bottom left" map
         {
            y = 0;
            h = ( ivy + ivh ) - mapH;
            layerFunc( game, x, y, w, h, 0, mapH - ivy );
         }
      }

      if ( ( ivx + ivw ) >= mapW ) // draw "right" map
      {
         x = 0;
         y = ivy;
         w = ( ivx + ivw ) - mapW;
         h = ivh;
         layerFunc( game, x, y, w, h, mapW - ivx, 0 );

         if ( ivy < 0 ) // draw "top right" map
         {
            y = mapH + ivy;
            h = -ivy;
            layerFunc( game, x, y, w, h, mapW - ivx, 0 );
         }

         if ( ( ivy + ivh ) > mapH ) // draw "bottom right" map
         {
            y = 0;
            h = ( ivy + ivh ) - mapH;
            layerFunc( game, x, y, w, h, mapW - ivx, mapH - ivy );
         }
      }

      x = ivx;
      w = ivw;

      if ( ivy < 0 ) // draw "top" map
      {
         y = mapH + ivy;
         h = -ivy;
         layerFunc( game, x, y, w, h, 0, 0 );
      }

      if ( ( ivy + ivh ) > mapH ) // draw "bottom" map
      {
         y = 0;
         h = ( ivy + ivh ) - mapH;
         layerFunc( game, x, y, w, h, 0, mapH - ivy );
      }
   }
}

internal void Render_DrawTileMapSection( Game_t* game, i32 vx, i32 vy, i32 vw, i32 vh, i32 xOffset, i32 yOffset )
{
   i32 tileOffsetX, tileOffsetY;
   i32 startTileX, endTileX, startTileY, endTileY;
   u32 row, col, x, y;
   u16* tile;

   // X values
   if ( vx < 0 )
   {
      startTileX = 0;
      tileOffsetX = -vx;
   }
   else
   {
      startTileX = vx / TILEMAP_TILE_SIZE;
      tileOffsetX = -( vx - ( startTileX * TILEMAP_TILE_SIZE ) );
   }

   endTileX = ( ( vx + vw ) / TILEMAP_TILE_SIZE );

   if ( endTileX >= (i32)( game->tileMap.tilesX ) )
   {
      endTileX = (i32)( game->tileMap.tilesX - 1 );
   }

   // Y values
   if ( vy < 0 )
   {
      startTileY = 0;
      tileOffsetY = -vy;
   }
   else
   {
      startTileY = vy / TILEMAP_TILE_SIZE;
      tileOffsetY = -( vy - ( startTileY * TILEMAP_TILE_SIZE ) );
   }

   endTileY = ( ( vy + vh ) / TILEMAP_TILE_SIZE );

   if ( endTileY >= (i32)( game->tileMap.tilesY ) )
   {
      endTileY = (i32)( game->tileMap.tilesY - 1 );
   }

   // actual drawing
   tile = game->tileMap.tiles + ( startTileY * game->tileMap.tilesX ) + startTileX;

   for ( row = 0, y = game->tileMap.viewportScreenPos.y + yOffset; row <= (u32)( endTileY - startTileY ); row++, y += TILEMAP_TILE_SIZE )
   {
      for ( col = 0, x = game->tileMap.viewportScreenPos.x + xOffset; col <= (u32)( endTileX - startTileX ); col++, x += TILEMAP_TILE_SIZE )
      {
         Screen_DrawBoundedBuffer8( &game->screen,
                                    game->tileMap.tileTextures[TILE_GET_TEXTURE_INDEX( *tile )].paletteIndexes,
                                    TILEMAP_TILE_SIZE, TILEMAP_TILE_SIZE,
                                    x + tileOffsetX, y + tileOffsetY,
                                    game->tileMap.viewportScreenPos.x + xOffset, game->tileMap.viewportScreenPos.y + yOffset,
                                    game->tileMap.viewportScreenPos.x + vw + xOffset,
                                    game->tileMap.viewportScreenPos.y + vh + yOffset );

         tile++;
      }

      tile += ( game->tileMap.tilesX - ( endTileX - startTileX + 1 ) );
   }
}

internal void Render_DrawStaticSpritesInSection( Game_t* game, i32 vx, i32 vy, i32 vw, i32 vh, i32 xOffset, i32 yOffset )
{
   u32 i;
   u32 spriteX, spriteY;
   StaticSprite_t* sprite = game->tileMap.staticSprites;
   Vector2u32_t* viewportScreenPos = &game->tileMap.viewportScreenPos;

   for ( i = 0; i < game->tileMap.staticSpriteCount; i++ )
   {
      TileMap_GetPositionOfTileIndex( &game->tileMap, sprite->tileIndex, &spriteX, &spriteY );

      if ( Utility_RectsIntersect32i( (i32)spriteX, (i32)spriteY, (i32)( spriteX + STATIC_SPRITE_SIZE ), (i32)( spriteY + STATIC_SPRITE_SIZE ), vx, vy, vw, vh) )
      {
         Screen_DrawBoundedBuffer8( &game->screen,
                                    game->tileMap.staticSpriteTextures[sprite->textureIndex].paletteIndexes,
                                    STATIC_SPRITE_SIZE, STATIC_SPRITE_SIZE,
                                    ( (i32)( spriteX ) - vx ) + viewportScreenPos->x + xOffset,
                                    ( (i32)( spriteY ) - vy ) + viewportScreenPos->y + yOffset,
                                    viewportScreenPos->x + xOffset, viewportScreenPos->y + yOffset,
                                    viewportScreenPos->x + vw + xOffset,
                                    viewportScreenPos->y + vh + yOffset );
      }

      sprite++;
   }
}

internal void Render_DrawEntitiesInSection( Game_t* game, i32 vx, i32 vy, i32 vw, i32 vh, i32 xOffset, i32 yOffset )
{
   Entity_t* sortedEntities[TILEMAP_MAX_ENTITIES + MAX_PLAYERS];
   u32 sortCount = 0;

   Render_SortEntities( game->tileMap.entities, game->tileMap.entityCount, sortedEntities, &sortCount, False );
   Render_SortEntities( game->tileMap.playerEntities, game->playerCount, sortedEntities, &sortCount, True );

   Render_DrawSortedEntities( game, sortedEntities, sortCount, vx, vy, vw, vh, xOffset, yOffset );
}

internal void Render_SortEntities( Entity_t* entities, u32 entityCount, Entity_t** sortedEntities, u32* sortCount, Bool_t reverseOrder )
{
   i32 i;

   if ( reverseOrder )
   {
      for ( i = ( (i32)entityCount - 1 ); i >= 0; i--, ( *sortCount )++ )
      {
         Render_SortEntity( entities + i, sortedEntities, sortCount );
      }
   }
   else
   {
      for ( i = 0; i < (i32)entityCount; i++, ( *sortCount )++ )
      {
         Render_SortEntity( entities + i, sortedEntities, sortCount );
      }
   }
}

internal void Render_SortEntity( Entity_t* entity, Entity_t** sortedEntities, u32* sortCount )
{
   u32 i, j;
   Bool_t inserted = False;

   for ( i = 0; i < *sortCount; i++ )
   {
      if ( entity->pos.y < sortedEntities[i]->pos.y )
      {
         for ( j = *sortCount; j > i; j-- )
         {
            sortedEntities[j] = sortedEntities[j - 1];
         }

         sortedEntities[i] = entity;
         inserted = True;
         break;
      }
   }

   if ( !inserted )
   {
      sortedEntities[*sortCount] = entity;
   }
}

internal void Render_DrawSortedEntities( Game_t* game, Entity_t** sortedEntities, u32 entityCount, i32 vx, i32 vy, i32 vw, i32 vh, i32 xOffset, i32 yOffset )
{
   u32 i, j, startPos;
   i32 ix, iy, iw, ih;
   ActiveSpriteTexture_t* textures;
   Entity_t* entity;
   Vector2u32_t* viewportScreenPos = &game->tileMap.viewportScreenPos;

   for ( i = 0; i < entityCount; i++ )
   {
      entity = sortedEntities[i];

      ix = entity->pos.x < 0.0f ? (i32)( entity->pos.x - 1.0f ) : (i32)entity->pos.x;
      iy = entity->pos.y < 0.0f ? (i32)( entity->pos.y - 1.0f ) : (i32)entity->pos.y;
      iw = (i32)entity->pos.w;
      ih = (i32)entity->pos.h;

      if ( Utility_RectsIntersect32i( ix, iy, iw, ih, vx, vy, vw, vh ) )
      {
         if ( entity->sprite->frame > 0 )
         {
            entity->sprite->frame = entity->sprite->frame;
         }

         startPos = ( ACTIVE_SPRITE_FRAME_PIXELS * ACTIVE_SPRITE_FRAMES ) * entity->sprite->direction + ( ACTIVE_SPRITE_FRAME_PIXELS * entity->sprite->frame );
         textures = 0;

         for ( j = 0; j < game->playerCount; j++ )
         {
            if ( entity == game->players[j].entity )
            {
               textures = game->tileMap.playerSpriteTextures;
               break;
            }
         }

         if ( !textures )
         {
            textures = game->tileMap.activeSpriteTextures;
         }

         Screen_DrawBoundedBuffer8( &game->screen,
                                    textures[entity->sprite->textureIndex].paletteIndexes + startPos,
                                    ACTIVE_SPRITE_FRAME_SIZE, ACTIVE_SPRITE_FRAME_SIZE,
                                    ( ix - vx - entity->sprite->offset.x ) + viewportScreenPos->x + xOffset,
                                    ( iy - vy - entity->sprite->offset.y ) + viewportScreenPos->y + yOffset,
                                    viewportScreenPos->x + xOffset, viewportScreenPos->y + yOffset,
                                    viewportScreenPos->x + vw + xOffset,
                                    viewportScreenPos->y + vh + yOffset );

#if defined( VISUAL_STUDIO_DEV )
         if ( g_winDebugFlags.showHitBoxes )
         {
            Screen_DrawBoundedRect( &game->screen,
                                    ( ix - vx ) + viewportScreenPos->x + xOffset,
                                    ( iy - vy ) + viewportScreenPos->y + yOffset,
                                    iw, ih,
                                    viewportScreenPos->x + xOffset, viewportScreenPos->y + yOffset,
                                    viewportScreenPos->x + vw + xOffset,
                                    viewportScreenPos->y + vh + yOffset,
                                    SCREEN_COLOR16_MAGENTA );
         }
#endif
      }
   }
}