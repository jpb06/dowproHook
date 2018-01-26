export interface GamePlayer {
    PRace: string;
    PHuman: number;
    PFnlState: number;
    PTeam: number;
    PName: string;
    PTtlSc: number;
}

export function isGamePlayer(obj: any): obj is GamePlayer {
    return (
        typeof obj.PRace === "string" &&
        typeof obj.PHuman === "number" &&
        typeof obj.PFnlState === "number" &&
        typeof obj.PTeam === "number" &&
        typeof obj.PName === "string" &&
        typeof obj.PTtlSc === "number"
    );
}