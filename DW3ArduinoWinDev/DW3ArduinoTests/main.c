#include "main.h"

#include <unity.h>

void setUp()
{
}

void tearDown()
{
}

int main( int argc, char** argv )
{
   UNITY_BEGIN();

   // test_input.c
   RUN_TEST( Input_Init_Always_SetsAllStatesToFalse );
   RUN_TEST( Input_AnyButtonPressed_NoButtonsPressed_ReturnsFalse );
   RUN_TEST( Input_AnyButtonPressed_ButtonWasPressed_ReturnsTrue );
   RUN_TEST( Input_HandleOverworldGeneral_OnlyLeftIsDown_VelocityIsCorrect );
   RUN_TEST( Input_HandleOverworldGeneral_OnlyUpIsDown_VelocityIsCorrect );
   RUN_TEST( Input_HandleOverworldGeneral_OnlyRightIsDown_VelocityIsCorrect );
   RUN_TEST( Input_HandleOverworldGeneral_OnlyDownIsDown_VelocityIsCorrect );
   RUN_TEST( Input_HandleOverworldGeneral_LeftAndUpAreDown_VelocitiesAreCorrect );
   RUN_TEST( Input_HandleOverworldGeneral_UpAndRightAreDown_VelocitiesAreCorrect );
   RUN_TEST( Input_HandleOverworldGeneral_RightAndDownAreDown_VelocitiesAreCorrect );
   RUN_TEST( Input_HandleOverworldGeneral_DownAndLeftAreDown_VelocitiesAreCorrect );
   RUN_TEST( Input_HandleOverworldGeneral_LeftIsDown_PlayerFacesLeft );
   RUN_TEST( Input_HandleOverworldGeneral_UpIsDown_PlayerFacesUp );
   RUN_TEST( Input_HandleOverworldGeneral_RightIsDown_PlayerFacesRight );
   RUN_TEST( Input_HandleOverworldGeneral_DownIsDown_PlayerFacesDown );
   RUN_TEST( Input_HandleOverworldGeneral_LeftAndUpAreDownAndPlayerIsFacingUp_PlayerKeepsFacingUp );
   RUN_TEST( Input_HandleOverworldGeneral_LeftAndUpAreDownAndPlayerIsNotFacingUp_PlayerFacesLeft );
   RUN_TEST( Input_HandleOverworldGeneral_UpAndRightAreDownAndPlayerIsFacingUp_PlayerKeepsFacingUp );
   RUN_TEST( Input_HandleOverworldGeneral_UpAndRightAreDownAndPlayerIsNotFacingUp_PlayerFacesRight );
   RUN_TEST( Input_HandleOverworldGeneral_RightAndDownAreDownAndPlayerIsFacingDown_PlayerKeepsFacingDown );
   RUN_TEST( Input_HandleOverworldGeneral_RightAndDownAreDownAndPlayerIsNotFacingDown_PlayerFacesRight );
   RUN_TEST( Input_HandleOverworldGeneral_DownAndLeftAreDownAndPlayerIsFacingDown_PlayerKeepsFacingDown );
   RUN_TEST( Input_HandleOverworldGeneral_DownAndLeftAreDownAndPlayerIsNotFacingDown_PlayerFacesLeft );
   RUN_TEST( Input_HandleOverworldGeneral_LeftAndRightAreDown_PlayerDirectionDoesNotChange );
   RUN_TEST( Input_HandleOverworldGeneral_UpAndDownAreDown_PlayerDirectionDoesNotChange );
   RUN_TEST( Input_HandleOverworldGeneral_LeftAndUpAndRightAreDownAndPlayerIsFacingHorizontal_PlayerKeepsFacingHorizontal );
   RUN_TEST( Input_HandleOverworldGeneral_LeftAndUpAndRightAreDownAndPlayerIsFacingDown_PlayerFacesUp );
   RUN_TEST( Input_HandleOverworldGeneral_LeftAndUpAndRightAreDownAndPlayerIsFacingUp_PlayerKeepsFacingUp );
   RUN_TEST( Input_HandleOverworldGeneral_UpAndRightAndDownAreDownAndPlayerIsFacingVertical_PlayerKeepsFacingVertical );
   RUN_TEST( Input_HandleOverworldGeneral_UpAndRightAndDownAreDownAndPlayerIsFacingLeft_PlayerFacesRight );
   RUN_TEST( Input_HandleOverworldGeneral_UpAndRightAndDownAreDownAndPlayerIsFacingRight_PlayerKeepsFacingRight );
   RUN_TEST( Input_HandleOverworldGeneral_RightAndDownAndLeftAreDownAndPlayerIsFacingHorizontal_PlayerKeepsFacingHorizontal );
   RUN_TEST( Input_HandleOverworldGeneral_RightAndDownAndLeftAreDownAndPlayerIsFacingUp_PlayerFacesDown );
   RUN_TEST( Input_HandleOverworldGeneral_RightAndDownAndLeftAreDownAndPlayerIsFacingDown_PlayerKeepsFacingDown );
   RUN_TEST( Input_HandleOverworldGeneral_DownAndLeftAndRightAreDownAndPlayerIsFacingHorizontal_PlayerKeepsFacingHorizontal );
   RUN_TEST( Input_HandleOverworldGeneral_DownAndLeftAndRightAreDownAndPlayerIsFacingUp_PlayerFacesDown );
   RUN_TEST( Input_HandleOverworldGeneral_DownAndLeftAndRightAreDownAndPlayerIsFacingDown_PlayerKeepsFacingDown );

   // test_npc.c
   RUN_TEST( Npc_Init_Always_InitializesBasicParameters );
   RUN_TEST( Npc_Init_NpcWanders_InitializesWanderParameters );
   RUN_TEST( Npc_Init_NpcDoesNotWander_DoesNotInitializeWanderParameters );
   RUN_TEST( Npc_Tic_DoesNotWander_DoesNothing );
   RUN_TEST( Npc_Tic_Wanders_IncrementsElapsedSeconds );
   RUN_TEST( Npc_Tic_WandersAndIsWanderingAndActionHasElapsed_StopsMoving );
   RUN_TEST( Npc_Tic_WandersAndIsNotWanderingAndActionHasElapsed_StartsMoving );
   RUN_TEST( Npc_Tic_FacingLeftAndNotMovingLeftAndMovingUp_SetsDirectionToUp );
   RUN_TEST( Npc_Tic_FacingLeftAndNotMovingLeftAndNotMovingUpAndMovingRight_SetsDirectionToRight );
   RUN_TEST( Npc_Tic_FacingLeftAndNotMovingLeftAndNotMovingUpAndNotMovingRight_SetsDirectionToDown );
   RUN_TEST( Npc_Tic_FacingUpAndNotMovingUpAndMovingRight_SetsDirectionToRight );
   RUN_TEST( Npc_Tic_FacingUpAndNotMovingUpAndNotMovingRightAndMovingDown_SetsDirectionToDown );
   RUN_TEST( Npc_Tic_FacingUpAndNotMovingUpAndNotMovingRightAndNotMovingDown_SetsDirectionToLeft );
   RUN_TEST( Npc_Tic_FacingRightAndNotMovingRightAndMovingDown_SetsDirectionToDown );
   RUN_TEST( Npc_Tic_FacingRightAndNotMovingRightAndNotMovingDownAndMovingLeft_SetsDirectionToLeft );
   RUN_TEST( Npc_Tic_FacingRightAndNotMovingRightAndNotMovingDownAndNotMovingLeft_SetsDirectionToUp );
   RUN_TEST( Npc_Tic_FacingDownAndNotMovingDownAndMovingLeft_SetsDirectionToLeft );
   RUN_TEST( Npc_Tic_FacingDownAndNotMovingDownAndNotMovingLeftAndMovingUp_SetsDirectionToUp );
   RUN_TEST( Npc_Tic_FacingDownAndNotMovingDownAndNotMovingLeftAndNotMovingUp_SetsDirectionToRight );

   // test_player.c
   RUN_TEST( Player_ResetChaining_ResetsParameters );

   // test_sprite.c
   RUN_TEST( ActiveSprite_Init_Always_InitializesParameters );
   RUN_TEST( ActiveSprite_SetDirection_DirectionHasNotChanged_DoesNotResetFrame );
   RUN_TEST( ActiveSprite_SetDirection_DirectionHasChanged_ResetsFrameAndChangesDirection );
   RUN_TEST( ActiveSprite_Tic_FrameSecondsHasNotExceededTotal_FrameAndFrameSecondsDoNotChange );
   RUN_TEST( ActiveSprite_Tic_FrameSecondsHasExceededTotal_ResetsFrameSecondsAndIncrementsFrame );
   RUN_TEST( ActiveSprite_Tic_LastFrameAndFrameSecondsHasExceededTotal_SetsFrameToZero );

   // test_tile_map.c
   RUN_TEST( TileMap_Init_Always_InitializesParameters );
   RUN_TEST( TileMap_Tic_Always_TicsNpcs );
   RUN_TEST( TileMap_Tic_Always_TicsActiveSprites );
   RUN_TEST( TileMap_Tic_Always_TicsPlayerSprites );
   RUN_TEST( TileMap_CenterEntityOnTile_Always_UpdatesEntityTileIndex );
   RUN_TEST( TileMap_CenterEntityOnTile_Always_CentersEntityOnCorrectTile );
   RUN_TEST( TileMap_GetTileIndexAtPosition_Always_ReturnsCorrectTileIndex );
   RUN_TEST( TileMap_GetPositionOfTileIndex_Always_ReturnsCorrectTileIndexPosition );
   RUN_TEST( TileMap_TileIndexIsEdgeTile_TileIndexIsNotEdgeTile_ReturnsFalse );
   RUN_TEST( TileMap_TileIndexIsEdgeTile_TileMapWraps_ReturnsFalse );
   RUN_TEST( TileMap_TileIndexIsEdgeTile_TileIndexIsLeftEdgeTile_ReturnsTrue );
   RUN_TEST( TileMap_TileIndexIsEdgeTile_TileIndexIsTopEdgeTile_ReturnsTrue );
   RUN_TEST( TileMap_TileIndexIsEdgeTile_TileIndexIsRightEdgeTile_ReturnsTrue );
   RUN_TEST( TileMap_TileIndexIsEdgeTile_TileIndexIsBottomEdgeTile_ReturnsTrue );
   RUN_TEST( TileMap_GetTileVelocity_TileHasNormalSpeedFactor_ReturnsNormalWalkSpeed );
   RUN_TEST( TileMap_GetTileVelocity_TileHasSlowSpeedFactor_ReturnsSlowWalkSpeed );
   RUN_TEST( TileMap_GetTileVelocity_TileHasVerySlowSpeedFactor_ReturnsVerySlowWalkSpeed );
   RUN_TEST( TileMap_GetTileVelocity_TileHasCrawlSpeedFactor_ReturnsCrawlWalkSpeed );
   RUN_TEST( TileMap_GetTileDiagonalVelocity_NormalWalkSpeed_ReturnsNormalDiagonalSpeed );
   RUN_TEST( TileMap_GetTileDiagonalVelocity_SlowWalkSpeed_ReturnsNormalDiagonalSpeed );
   RUN_TEST( TileMap_GetTileDiagonalVelocity_VerySlowWalkSpeed_ReturnsNormalDiagonalSpeed );
   RUN_TEST( TileMap_GetTileDiagonalVelocity_CrawlWalkSpeed_ReturnsNormalDiagonalSpeed );
   RUN_TEST( TileMap_GetTileDiagonalVelocity_UnknownWalkSpeed_ReturnsCalculatedDiagonalSpeed );
   RUN_TEST( TileMap_ClampViewportToEntity_EdgeOfMapAndTileMapWraps_ClampsToFocalEntity );
   RUN_TEST( TileMap_ClampViewportToEntity_MapIsThinnerThanViewport_ClampsHorizontally );
   RUN_TEST( TileMap_ClampViewportToEntity_MapIsShorterThanViewport_ClampsVertically );
   RUN_TEST( TileMap_ClampViewportToEntity_CloseToLeftEdge_ClampsToScreen );
   RUN_TEST( TileMap_ClampViewportToEntity_CloseToTopEdge_ClampsToScreen );
   RUN_TEST( TileMap_ClampViewportToEntity_CloseToRightEdge_ClampsToScreen );
   RUN_TEST( TileMap_ClampViewportToEntity_CloseToBottomEdge_ClampsToScreen );
   RUN_TEST( TileMap_ClampViewportToEntity_NotCloseToEdge_ClampsToEntity );

   // test_validate_save.c
   RUN_TEST( Validate_PlayerCount_TooFewPlayers_ReturnsFalse );
   RUN_TEST( Validate_PlayerCount_TooManyPlayers_ReturnsFalse );
   RUN_TEST( Validate_PlayerCount_EnoughPlayers_ReturnsTrue );
   RUN_TEST( Validate_PlayerName_TooShort_ReturnsFalse );
   RUN_TEST( Validate_PlayerName_TooLong_ReturnsFalse );
   RUN_TEST( Validate_PlayerName_GoodLength_ReturnsTrue );
   RUN_TEST( Validate_PlayerClass_TooLow_ReturnsFalse );
   RUN_TEST( Validate_PlayerClass_TooHigh_ReturnsFalse );
   RUN_TEST( Validate_PlayerClass_GoodClass_ReturnsTrue );
   RUN_TEST( Validate_SingleHero_NoHeroes_ReturnsFalse );
   RUN_TEST( Validate_SingleHero_MultipleHeroes_ReturnsFalse );
   RUN_TEST( Validate_SingleHero_OneHero_ReturnsTrue );

   return UNITY_END();
}
