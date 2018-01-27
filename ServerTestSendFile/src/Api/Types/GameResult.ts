import { GamePlayer, isGamePlayer } from './GamePlayer';

export interface GameResult {
    Players: string;
    WinBy: string;
    Teams: number;
    Duration: number;
    Scenario: string;
    player_0: GamePlayer;
    player_1: GamePlayer;
}

export function isGameResult(obj: any): obj is GameResult {
    return (
        typeof obj.Players === "string" &&
        typeof obj.WinBy === "string" &&
        typeof obj.Teams === "number" &&
        typeof obj.Duration === "number" &&
        typeof obj.Scenario === "string" &&
        isGamePlayer(obj.player_0) &&
        isGamePlayer(obj.player_1)
    );
}